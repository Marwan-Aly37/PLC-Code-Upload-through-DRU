"""DRU firmware sender with IEC response handling and binary integrity CRC verification."""

import base64
import ctypes
import math
import os
import queue
import struct
import threading
import time
import tkinter as tk
import zlib
from dataclasses import dataclass
from enum import Enum
from tkinter import filedialog, messagebox, ttk

import serial
import serial.tools.list_ports

from configuration.config import (
    BAUD_CODES,
    CODE_TO_BAUD,
    DEFAULT_FIRMWARE_TYPE,
    FIRMWARE_COMMANDS,
    INITIAL_BAUD,
    LENGTH_BYTEORDER,
    LENGTH_OVERHEAD,
    STX,
)
from utils.utils import (
    build_baud_command,
    build_data_frame,
    build_init_frame,
    build_termination_frame,
    calculate_bcc,
    format_hex_dump,
    validate_protocol_frame,
)


ACK_RESPONSE = b"\x06A\r\n"
IEC_REJECT_RESPONSE = b"\x06R\r\n"
LEGACY_NAK_RESPONSE = b"\x15N\r\n"
RESPONSE_MARKERS = (
    ACK_RESPONSE,
    IEC_REJECT_RESPONSE,
    LEGACY_NAK_RESPONSE,
)

IDENTIFICATION_REQUEST = b"/?!\r\n"
VIRTUAL_PORT = "VIRTUAL_DEVICE (Simulation)"

ACK_TIMEOUT_SECONDS = 5.0
IDENTIFICATION_TIMEOUT_SECONDS = 3.0
METADATA_TIMEOUT_SECONDS = 3.0
BAUD_CHANGE_DELAY_SECONDS = 0.05
HANDSHAKE_BITS_PER_CHARACTER = 10
METADATA_QUIET_SECONDS = 0.25

DATA_BLOCK_SIZE = 254
SAVED_HEADER_SIZE = 7
SUPPORTED_FIRMWARE_COMMANDS = frozenset(FIRMWARE_COMMANDS.values())

# MCU CRC Constants
INTEGRITY_CRC_NUM = 981
INTEGRITY_CRC_ADDRESS = 0x3D5000
CRC_WIDTH = 8
CRC_TOPBIT = 0x80
CRC_POLYNOMIAL = 0x07  # Change if your firmware uses a specific polynomial


class TransferError(Exception):
    """Base error for an unsuccessful transfer."""


class IECRejectError(TransferError):
    """Raised when the MCU returns 06 52 0D 0A."""


class TransferCancelled(TransferError):
    """Raised when the operator cancels the transfer."""


class IECResponse(Enum):
    ACK = "ack"
    REJECT = "reject"
    NAK = "nak"
    TIMEOUT = "timeout"
    UNKNOWN = "unknown"


def calc_crc(data_ptr: bytes, calc_type: str = "CRC", polynomial: int = CRC_POLYNOMIAL) -> int:
    """Exact python implementation of MCU's calc_crc routine."""
    remainder = 0
    size = len(data_ptr)

    if calc_type == "checksum":
        for byte in data_ptr:
            remainder ^= byte
    elif calc_type == "CRC":
        for byte in data_ptr:
            remainder ^= (byte << (CRC_WIDTH - 8)) & 0xFF
            for _ in range(8):
                if remainder & CRC_TOPBIT:
                    remainder = ((remainder << 1) ^ polynomial) & 0xFF
                else:
                    remainder = (remainder << 1) & 0xFF

    if remainder == 0:
        if all(b == 0 for b in data_ptr):
            return 0x55

    return remainder


@dataclass(frozen=True)
class TransferPlan:
    file_size: int
    block_count: int
    stored_size: int
    crc: int
    crc_hex: str


def classify_iec_response(data):
    """Classify the short response returned by the IEC state machine."""
    if IEC_REJECT_RESPONSE in data:
        return IECResponse.REJECT
    if LEGACY_NAK_RESPONSE in data:
        return IECResponse.NAK
    if ACK_RESPONSE in data:
        return IECResponse.ACK
    if not data:
        return IECResponse.TIMEOUT
    return IECResponse.UNKNOWN


def make_transfer_plan(firmware_data: bytes):
    """Calculate packet count, occupied internal-flash bytes, and integrity CRC."""
    file_size = len(firmware_data)
    if file_size <= 0:
        raise ValueError("The selected BIN file is empty.")

    block_count = math.ceil(file_size / DATA_BLOCK_SIZE)
    stored_size = file_size + (block_count * SAVED_HEADER_SIZE)
    crc_val = calc_crc(firmware_data, calc_type="CRC")
    crc_hex = f"0x{crc_val:02X}"
    return TransferPlan(file_size, block_count, stored_size, crc_val, crc_hex)


def format_bytes(value):
    if value >= 1024:
        return f"{value:,} B ({value / 1024:.1f} KiB)"
    return f"{value:,} B"


class SmoothProgress(tk.Canvas):
    """Rounded progress bar with smooth movement and a soft active sweep."""

    def __init__(self, master, **kwargs):
        self.track_color = kwargs.pop("track_color", "#DDE4E8")
        self.fill_color = kwargs.pop("fill_color", "#087E8B")
        self.shimmer_color = kwargs.pop("shimmer_color", "#4CB7BE")
        self.canvas_color = kwargs.pop("canvas_color", "#FFFFFF")
        super().__init__(
            master,
            height=14,
            highlightthickness=0,
            borderwidth=0,
            background=self.canvas_color,
            **kwargs,
        )
        self.target_value = 0.0
        self.display_value = 0.0
        self.active = False
        self.shimmer_x = -50
        self.bind("<Configure>", lambda _event: self._draw())
        self.after(24, self._animate)

    def set_value(self, value, immediate=False):
        self.target_value = max(0.0, min(100.0, float(value)))
        if immediate:
            self.display_value = self.target_value
            self._draw()

    def set_active(self, active):
        self.active = bool(active)
        if not active:
            self.shimmer_x = -50
        self._draw()

    def _animate(self):
        difference = self.target_value - self.display_value
        if abs(difference) > 0.05:
            self.display_value += difference * 0.16
        else:
            self.display_value = self.target_value

        if self.active:
            self.shimmer_x += 9
            if self.shimmer_x > self.winfo_width() + 50:
                self.shimmer_x = -50

        self._draw()
        try:
            self.after(24, self._animate)
        except tk.TclError:
            pass

    def _draw(self):
        width = max(1, self.winfo_width())
        height = max(1, self.winfo_height())
        fill_width = int(width * self.display_value / 100.0)
        self.delete("all")
        self._rounded_rectangle(0, 1, width, height - 1, self.track_color)
        if fill_width > 0:
            self._rounded_rectangle(0, 1, fill_width, height - 1, self.fill_color)
        if self.active and fill_width > 18:
            left = max(0, self.shimmer_x - 42)
            right = min(fill_width, self.shimmer_x)
            if right > left:
                self.create_rectangle(
                    left,
                    2,
                    right,
                    height - 2,
                    fill=self.shimmer_color,
                    outline="",
                )

    def _rounded_rectangle(self, left, top, right, bottom, color):
        if right <= left:
            return
        radius = max(1, min((bottom - top) // 2, (right - left) // 2))
        self.create_rectangle(
            left + radius, top, right - radius, bottom, fill=color, outline=""
        )
        self.create_rectangle(
            left, top + radius, right, bottom - radius, fill=color, outline=""
        )
        self.create_oval(
            left, top, left + (radius * 2), bottom, fill=color, outline=""
        )
        self.create_oval(
            right - (radius * 2), top, right, bottom, fill=color, outline=""
        )


class PulseIndicator(tk.Canvas):
    """Small animated indicator for live transfer state."""

    COLORS = {
        "idle": "#87949D",
        "running": "#087E8B",
        "success": "#287A52",
        "error": "#B42318",
        "warning": "#B87818",
    }

    def __init__(self, master, background):
        super().__init__(
            master,
            width=28,
            height=28,
            background=background,
            highlightthickness=0,
            borderwidth=0,
        )
        self.state = "idle"
        self.phase = 0
        self.after(70, self._animate)

    def set_state(self, state):
        self.state = state
        self.phase = 0
        self._draw()

    def _animate(self):
        if self.state == "running":
            self.phase = (self.phase + 1) % 16
        self._draw()
        try:
            self.after(70, self._animate)
        except tk.TclError:
            pass

    def _draw(self):
        self.delete("all")
        color = self.COLORS.get(self.state, self.COLORS["idle"])
        if self.state == "running":
            distance = self.phase if self.phase <= 8 else 16 - self.phase
            radius = 7 + int(distance * 0.5)
            self.create_oval(
                14 - radius,
                14 - radius,
                14 + radius,
                14 + radius,
                outline="#8ACDD1",
                width=1,
            )
        self.create_oval(10, 10, 18, 18, fill=color, outline="")


class StageTimeline(tk.Canvas):
    """Animated transfer timeline drawn as one stable responsive control."""

    def __init__(self, master, stages, colors, **kwargs):
        self.stages = tuple(stages)
        self.colors = colors
        self.active_index = 0
        self.error = False
        self.complete = False
        self.phase = 0
        super().__init__(
            master,
            height=66,
            background=colors["background"],
            highlightthickness=0,
            borderwidth=0,
            **kwargs,
        )
        self.bind("<Configure>", lambda _event: self._draw())
        self.after(55, self._animate)

    def set_stage(self, active_index, error=False):
        self.active_index = max(0, min(len(self.stages) - 1, active_index))
        self.error = bool(error)
        self.complete = False
        self.phase = 0
        self._draw()

    def set_complete(self):
        self.active_index = len(self.stages) - 1
        self.error = False
        self.complete = True
        self._draw()

    def _animate(self):
        self.phase = (self.phase + 1) % 24
        self._draw()
        try:
            self.after(55, self._animate)
        except tk.TclError:
            pass

    def _draw(self):
        self.delete("all")
        width = max(1, self.winfo_width())
        side = 58
        usable = max(1, width - (side * 2))
        step = usable / max(1, len(self.stages) - 1)
        centers = [side + (step * index) for index in range(len(self.stages))]
        line_y = 20

        self.create_line(
            centers[0], line_y, centers[-1], line_y,
            fill=self.colors["border"], width=3,
        )
        if self.active_index > 0 or self.complete:
            line_end = centers[-1] if self.complete else centers[self.active_index]
            self.create_line(
                centers[0], line_y, line_end, line_y,
                fill=self.colors["green"] if self.complete else self.colors["accent"],
                width=3,
            )

        for index, (center, label) in enumerate(zip(centers, self.stages)):
            if self.error and index == self.active_index:
                fill = self.colors["red"]
                foreground = "!"
            elif index < self.active_index or self.complete:
                fill = self.colors["green"]
                foreground = "\u2713"
            elif index == self.active_index:
                fill = self.colors["accent"]
                foreground = str(index + 1)
                pulse = 12 + min(self.phase, 24 - self.phase) // 4
                self.create_oval(
                    center - pulse, line_y - pulse,
                    center + pulse, line_y + pulse,
                    outline=self.colors["accent_soft"], width=2,
                )
            else:
                fill = self.colors["surface"]
                foreground = str(index + 1)

            outline = fill if index <= self.active_index else self.colors["border"]
            text_color = "#FFFFFF" if index <= self.active_index else self.colors["muted"]
            self.create_oval(
                center - 10, line_y - 10, center + 10, line_y + 10,
                fill=fill, outline=outline, width=2,
            )
            self.create_text(
                center, line_y, text=foreground, fill=text_color,
                font=("Segoe UI Semibold", 8),
            )
            label_color = (
                self.colors["red"] if self.error and index == self.active_index
                else self.colors["text"] if index <= self.active_index
                else self.colors["muted"]
            )
            self.create_text(
                center, 50, text=label, fill=label_color,
                font=("Segoe UI Semibold" if index == self.active_index else "Segoe UI", 9),
            )


class ModernButton(tk.Canvas):
    """Rounded command button with animated hover and keyboard activation."""

    def __init__(
        self,
        master,
        text,
        command,
        colors,
        kind="secondary",
        width=108,
        height=38,
    ):
        self.text = text
        self.command = command
        self.colors = colors
        self.kind = kind
        self.state = "normal"
        self.hover_value = 0.0
        self.hover_target = 0.0
        self.focused = False
        self._image_cache = {}
        self._button_image = None
        canvas_color = colors["surface"]
        super().__init__(
            master,
            width=width,
            height=height,
            background=canvas_color,
            highlightthickness=0,
            borderwidth=0,
            cursor="hand2",
            takefocus=1,
        )
        self.bind("<Enter>", self._on_enter)
        self.bind("<Leave>", self._on_leave)
        self.bind("<ButtonPress-1>", self._on_press)
        self.bind("<ButtonRelease-1>", self._on_release)
        self.bind("<FocusIn>", self._on_focus)
        self.bind("<FocusOut>", self._on_blur)
        self.bind("<Return>", self._on_keyboard)
        self.bind("<space>", self._on_keyboard)
        self.bind("<Configure>", lambda _event: self._draw())
        self.after(20, self._animate)
        self._draw()

    def configure(self, cnf=None, **kwargs):
        options = dict(cnf or {})
        options.update(kwargs)
        if "state" in options:
            self.state = str(options.pop("state"))
            self.configure(cursor="" if self.state == "disabled" else "hand2")
        if "text" in options:
            self.text = str(options.pop("text"))
        if "command" in options:
            self.command = options.pop("command")
        if options:
            super().configure(**options)
        self._draw()

    config = configure

    def _on_enter(self, _event):
        if self.state != "disabled":
            self.hover_target = 1.0

    def _on_leave(self, _event):
        self.hover_target = 0.0

    def _on_press(self, _event):
        if self.state != "disabled":
            self.hover_value = 1.0
            self._draw(pressed=True)

    def _on_release(self, event):
        if self.state == "disabled":
            return
        self._draw()
        if 0 <= event.x <= self.winfo_width() and 0 <= event.y <= self.winfo_height():
            self.focus_set()
            if callable(self.command):
                self.command()

    def _on_focus(self, _event):
        self.focused = True
        self._draw()

    def _on_blur(self, _event):
        self.focused = False
        self._draw()

    def _on_keyboard(self, _event):
        if self.state != "disabled" and callable(self.command):
            self.command()
        return "break"

    def _animate(self):
        difference = self.hover_target - self.hover_value
        if abs(difference) > 0.02:
            self.hover_value += difference * 0.22
            self._draw()
        else:
            self.hover_value = self.hover_target
        try:
            self.after(20, self._animate)
        except tk.TclError:
            pass

    @staticmethod
    def _blend(first, second, amount):
        first_rgb = tuple(int(first[index:index + 2], 16) for index in (1, 3, 5))
        second_rgb = tuple(int(second[index:index + 2], 16) for index in (1, 3, 5))
        mixed = tuple(
            round(start + ((end - start) * amount))
            for start, end in zip(first_rgb, second_rgb)
        )
        return "#%02X%02X%02X" % mixed

    def _palette(self):
        if self.state == "disabled":
            return "#EFF2F4", "#9CA8AE", "#DCE2E5"
        if self.kind == "primary":
            return self.colors["accent"], "#FFFFFF", self.colors["accent"]
        if self.kind == "danger":
            return self.colors["surface"], self.colors["red"], "#E5C1BE"
        return self.colors["surface"], self.colors["text"], self.colors["border"]

    @staticmethod
    def _rounded_coverage(x, y, left, top, right, bottom, radius):
        center_x = (left + right) / 2.0
        center_y = (top + bottom) / 2.0
        half_width = (right - left) / 2.0
        half_height = (bottom - top) / 2.0
        corner_x = abs(x - center_x) - (half_width - radius)
        corner_y = abs(y - center_y) - (half_height - radius)
        outside = math.hypot(max(corner_x, 0.0), max(corner_y, 0.0))
        inside = min(max(corner_x, corner_y), 0.0)
        distance = outside + inside - radius
        return max(0.0, min(1.0, 0.5 - distance))

    @staticmethod
    def _rgb(color):
        return tuple(int(color[index:index + 2], 16) for index in (1, 3, 5))

    @staticmethod
    def _png_chunk(chunk_type, data):
        checksum = zlib.crc32(chunk_type + data) & 0xFFFFFFFF
        return (
            struct.pack(">I", len(data))
            + chunk_type
            + data
            + struct.pack(">I", checksum)
        )

    def _make_button_image(self, width, height, radius, border_width, fill, outline):
        cache_key = (width, height, radius, border_width, fill, outline)
        cached_image = self._image_cache.get(cache_key)
        if cached_image is not None:
            return cached_image

        background_rgb = self._rgb(self.colors["surface"])
        outline_rgb = self._rgb(outline)
        fill_rgb = self._rgb(fill)
        pixels = bytearray()

        outer_left = 0.75
        outer_top = 0.75
        outer_right = width - 0.75
        outer_bottom = height - 0.75
        inner_left = outer_left + border_width
        inner_top = outer_top + border_width
        inner_right = outer_right - border_width
        inner_bottom = outer_bottom - border_width
        inner_radius = max(1.0, radius - border_width)

        for pixel_y in range(height):
            pixels.append(0)
            y = pixel_y + 0.5
            for pixel_x in range(width):
                x = pixel_x + 0.5
                outer = self._rounded_coverage(
                    x, y, outer_left, outer_top, outer_right, outer_bottom, radius
                )
                inner = min(
                    outer,
                    self._rounded_coverage(
                        x,
                        y,
                        inner_left,
                        inner_top,
                        inner_right,
                        inner_bottom,
                        inner_radius,
                    ),
                )
                background_weight = 1.0 - outer
                outline_weight = outer - inner
                for channel in range(3):
                    value = (
                        background_rgb[channel] * background_weight
                        + outline_rgb[channel] * outline_weight
                        + fill_rgb[channel] * inner
                    )
                    pixels.append(round(value))

        header = struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)
        png_data = (
            b"\x89PNG\r\n\x1a\n"
            + self._png_chunk(b"IHDR", header)
            + self._png_chunk(b"IDAT", zlib.compress(bytes(pixels), 9))
            + self._png_chunk(b"IEND", b"")
        )
        image = tk.PhotoImage(master=self, data=base64.b64encode(png_data), format="png")
        if len(self._image_cache) >= 48:
            self._image_cache.clear()
        self._image_cache[cache_key] = image
        return image

    def _draw(self, pressed=False):
        self.delete("all")
        width = max(1, self.winfo_width())
        height = max(1, self.winfo_height())
        fill, foreground, border = self._palette()
        if self.state != "disabled":
            if self.kind == "primary":
                hover = self.colors["accent_dark"]
            elif self.kind == "danger":
                hover = "#FBEDEC"
            else:
                hover = self.colors["surface_hover"]
            fill = self._blend(fill, hover, self.hover_value)
        if pressed:
            fill = self._blend(fill, "#000000", 0.08)

        radius = 8
        outline = self.colors["accent"] if self.focused else border
        border_width = 2 if self.focused else 1
        self._button_image = self._make_button_image(
            width, height, radius, border_width, fill, outline
        )
        self.create_image(0, 0, anchor="nw", image=self._button_image)
        self.create_text(
            width / 2,
            (height / 2) + (1 if pressed else 0),
            text=self.text,
            fill=foreground,
            font=("Segoe UI Variable Text Semibold", 9),
        )


class VirtualSerialDevice:
    """Local target simulation used to verify the complete UI workflow."""

    METADATA_FRAME = bytes.fromhex(
        "02 2F 00 1E 00 00 00 06 08 1A 34 2E 35 32 36 00 "
        "00 00 00 00 00 00 00 00 00 00 71 F7 0B 00 00 00 "
        "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 "
        "21 0D 0A 03 26"
    )

    def __init__(self, reject_data_packet=None):
        self.input_buffer = bytearray()
        self.output_buffer = bytearray()
        self.is_open = True
        self.baudrate = INITIAL_BAUD
        self.bytesize = serial.SEVENBITS
        self.parity = serial.PARITY_EVEN
        self.stopbits = serial.STOPBITS_ONE
        self.reject_data_packet = reject_data_packet
        self.data_packet_count = 0

    @property
    def in_waiting(self):
        return len(self.output_buffer)

    def write(self, data):
        if not self.is_open:
            raise serial.SerialException("Virtual port is closed.")
        self.input_buffer.extend(data)
        self._process_input()
        return len(data)

    def read(self, size=1):
        if not self.is_open:
            raise serial.SerialException("Virtual port is closed.")
        count = min(size, len(self.output_buffer))
        data = bytes(self.output_buffer[:count])
        del self.output_buffer[:count]
        return data

    def flush(self):
        return None

    def reset_input_buffer(self):
        self.output_buffer.clear()

    def close(self):
        self.is_open = False

    def _queue(self, data):
        self.output_buffer.extend(data)

    def _process_input(self):
        current = bytes(self.input_buffer)

        if current == IDENTIFICATION_REQUEST:
            self.input_buffer.clear()
            self._queue(b"/SEE4000000100\r\n")
            return

        if (
            len(current) == 6
            and current.startswith(b"\x060")
            and current.endswith(b"7\r\n")
        ):
            baud_code = chr(current[2])
            if baud_code in CODE_TO_BAUD:
                self.baudrate = CODE_TO_BAUD[baud_code]
            self.input_buffer.clear()
            self._queue(self.METADATA_FRAME)
            return

        while len(self.input_buffer) >= 3 and self.input_buffer[0] == STX:
            frame_size = (
                int.from_bytes(
                    self.input_buffer[1:3], byteorder=LENGTH_BYTEORDER
                )
                + LENGTH_OVERHEAD
            )
            if len(self.input_buffer) < frame_size:
                return

            frame = bytes(self.input_buffer[:frame_size])
            del self.input_buffer[:frame_size]
            valid, _reason = validate_protocol_frame(frame)

            is_data_packet = (
                len(frame) > 14 and frame[6] in SUPPORTED_FIRMWARE_COMMANDS
            )
            if is_data_packet:
                self.data_packet_count += 1
                valid = valid and frame[7] == 0x05
                valid = valid and frame[10:14] == b"\x00\x00\x05\x08"

            is_firmware_termination = (
                len(frame) >= 12
                and frame[3] == 0x00
                and frame[6] in SUPPORTED_FIRMWARE_COMMANDS
            )
            if valid and is_firmware_termination:
                self.data_packet_count = 0

            should_reject = (
                is_data_packet
                and self.reject_data_packet == self.data_packet_count
            )
            self._queue(
                ACK_RESPONSE if valid and not should_reject else IEC_REJECT_RESPONSE
            )


class DRUFlasherApp(tk.Tk):
    """Professional desktop interface for the firmware sender."""

    COLORS = {
        "background": "#F5F7F9",
        "surface": "#FFFFFF",
        "surface_alt": "#EEF3F5",
        "surface_hover": "#E5ECEF",
        "border": "#D8E0E5",
        "text": "#18232D",
        "muted": "#657580",
        "accent": "#0A7F82",
        "accent_dark": "#08696C",
        "accent_soft": "#D9F1EF",
        "blue": "#2D6FB7",
        "green": "#237D57",
        "amber": "#A96E16",
        "red": "#B42318",
        "log": "#111A22",
    }

    STAGES = ("Connect", "Negotiate", "Initialize", "Transfer", "Complete")

    def __init__(self):
        super().__init__()
        self.title("DRU Firmware Sender")
        self.geometry("1180x820")
        self.minsize(1000, 720)
        self.configure(background=self.COLORS["background"])

        self.cancel_requested = threading.Event()
        self.ui_events = queue.Queue()
        self.worker = None

        self.port_var = tk.StringVar()
        self.baud_var = tk.StringVar(value="9600")
        self.firmware_type_var = tk.StringVar(value=DEFAULT_FIRMWARE_TYPE)
        self.file_var = tk.StringVar()
        self.file_size_var = tk.StringVar(value="-")
        self.packet_count_var = tk.StringVar(value="-")
        self.crc_var = tk.StringVar(value="-")
        self.status_var = tk.StringVar(value="Ready")
        self.command_var = tk.StringVar(value="CMD 212  |  0xD4")
        self.progress_text_var = tk.StringVar(value="0%")
        self.verbose_var = tk.BooleanVar(value=False)

        self._configure_styles()
        self._create_widgets()
        self._on_firmware_type_change()
        self.refresh_ports()
        self.after(40, self._process_ui_events)
        self.after(20, self._fade_in)
        self.after(0, self._apply_windows_11_style)
        self.protocol("WM_DELETE_WINDOW", self._close_window)

    def _configure_styles(self):
        style = ttk.Style(self)
        style.theme_use("clam")
        style.configure(
            ".",
            background=self.COLORS["background"],
            foreground=self.COLORS["text"],
            font=("Segoe UI Variable Text", 10),
        )
        style.configure("TFrame", background=self.COLORS["background"])
        style.configure("Surface.TFrame", background=self.COLORS["surface"])
        style.configure("SurfaceAlt.TFrame", background=self.COLORS["surface_alt"])
        style.configure(
            "Title.TLabel",
            background=self.COLORS["background"],
            foreground=self.COLORS["text"],
            font=("Segoe UI Variable Display Semibold", 24),
        )
        style.configure(
            "Subtitle.TLabel",
            background=self.COLORS["background"],
            foreground=self.COLORS["muted"],
            font=("Segoe UI Variable Text", 10),
        )
        style.configure(
            "Status.TLabel",
            background=self.COLORS["surface_alt"],
            foreground=self.COLORS["text"],
            font=("Segoe UI Variable Text Semibold", 10),
        )
        style.configure(
            "StatusDetail.TLabel",
            background=self.COLORS["surface_alt"],
            foreground=self.COLORS["muted"],
            font=("Segoe UI Variable Text", 9),
        )
        style.configure(
            "TLabelframe",
            background=self.COLORS["surface"],
            bordercolor=self.COLORS["border"],
            borderwidth=1,
            relief="solid",
        )
        style.configure(
            "TLabelframe.Label",
            background=self.COLORS["surface"],
            foreground=self.COLORS["text"],
            font=("Segoe UI Variable Text Semibold", 10),
        )
        style.configure(
            "Surface.TLabel",
            background=self.COLORS["surface"],
            foreground=self.COLORS["text"],
        )
        style.configure(
            "Muted.TLabel",
            background=self.COLORS["surface"],
            foreground=self.COLORS["muted"],
        )
        style.configure(
            "Metric.TLabel",
            background=self.COLORS["surface"],
            foreground=self.COLORS["text"],
            font=("Segoe UI Variable Text Semibold", 11),
        )
        style.configure(
            "Command.TLabel",
            background=self.COLORS["surface"],
            foreground=self.COLORS["accent"],
            font=("Cascadia Mono SemiBold", 10),
        )
        style.configure(
            "ActionTitle.TLabel",
            background=self.COLORS["surface"],
            foreground=self.COLORS["text"],
            font=("Segoe UI Variable Text Semibold", 11),
        )
        style.configure(
            "ActionDetail.TLabel",
            background=self.COLORS["surface"],
            foreground=self.COLORS["muted"],
            font=("Segoe UI Variable Text", 9),
        )
        style.configure(
            "TButton",
            padding=(13, 8),
            borderwidth=1,
            background=self.COLORS["surface"],
            bordercolor=self.COLORS["border"],
            font=("Segoe UI Variable Text Semibold", 9),
        )
        style.map(
            "TButton",
            background=[("active", self.COLORS["surface_hover"])],
            bordercolor=[("active", "#BFCBD1")],
        )
        style.configure(
            "Accent.TButton",
            background=self.COLORS["accent"],
            foreground="#FFFFFF",
            bordercolor=self.COLORS["accent"],
            padding=(18, 10),
            font=("Segoe UI Variable Text Semibold", 10),
        )
        style.map(
            "Accent.TButton",
            background=[
                ("active", self.COLORS["accent_dark"]),
                ("disabled", "#A8B4BA"),
            ],
            bordercolor=[("active", self.COLORS["accent_dark"])],
        )
        style.configure(
            "Danger.TButton",
            background=self.COLORS["surface"],
            foreground=self.COLORS["red"],
            bordercolor="#E5C1BE",
            padding=(14, 10),
        )
        style.map(
            "Danger.TButton",
            background=[("active", "#FBEDEC"), ("disabled", "#F3F5F6")],
            foreground=[("disabled", "#9EA9AF")],
        )
        style.configure(
            "TCombobox",
            padding=(8, 7),
            fieldbackground=self.COLORS["surface"],
            background=self.COLORS["surface"],
            bordercolor=self.COLORS["border"],
            lightcolor=self.COLORS["border"],
            darkcolor=self.COLORS["border"],
            arrowcolor=self.COLORS["muted"],
        )
        style.map(
            "TCombobox",
            fieldbackground=[
                ("readonly", self.COLORS["surface"]),
                ("disabled", self.COLORS["surface_alt"]),
            ],
            foreground=[
                ("readonly", self.COLORS["text"]),
                ("disabled", "#9AA6AD"),
            ],
            bordercolor=[("focus", self.COLORS["accent"])],
            lightcolor=[("focus", self.COLORS["accent"])],
            darkcolor=[("focus", self.COLORS["accent"])],
        )
        style.configure(
            "TEntry",
            padding=(9, 8),
            fieldbackground=self.COLORS["surface"],
            bordercolor=self.COLORS["border"],
            lightcolor=self.COLORS["border"],
            darkcolor=self.COLORS["border"],
        )
        style.layout("Segment.TRadiobutton", style.layout("TButton"))
        style.configure(
            "Segment.TRadiobutton",
            padding=(20, 8),
            background=self.COLORS["surface_alt"],
            foreground=self.COLORS["muted"],
            bordercolor=self.COLORS["border"],
            font=("Segoe UI Variable Text Semibold", 9),
        )
        style.map(
            "Segment.TRadiobutton",
            background=[
                ("selected", self.COLORS["accent"]),
                ("active", self.COLORS["surface_hover"]),
                ("selected active", self.COLORS["accent_dark"]),
            ],
            foreground=[("selected", "#FFFFFF"), ("active", self.COLORS["text"])],
            bordercolor=[("selected", self.COLORS["accent"])],
        )

    def _apply_windows_11_style(self):
        if os.name != "nt":
            return
        try:
            self.update_idletasks()
            window_handle = ctypes.windll.user32.GetParent(self.winfo_id())
            rounded = ctypes.c_int(2)
            mica = ctypes.c_int(2)
            light_title = ctypes.c_int(0)
            ctypes.windll.dwmapi.DwmSetWindowAttribute(
                window_handle, 33, ctypes.byref(rounded), ctypes.sizeof(rounded)
            )
            ctypes.windll.dwmapi.DwmSetWindowAttribute(
                window_handle, 38, ctypes.byref(mica), ctypes.sizeof(mica)
            )
            ctypes.windll.dwmapi.DwmSetWindowAttribute(
                window_handle, 20, ctypes.byref(light_title), ctypes.sizeof(light_title)
            )
        except (AttributeError, OSError):
            pass

    def _create_widgets(self):
        accent_strip = tk.Frame(
            self, height=4, background=self.COLORS["accent"], borderwidth=0
        )
        accent_strip.pack(fill="x", side="top")

        root = ttk.Frame(self, padding=(28, 20, 28, 22))
        root.pack(fill="both", expand=True)
        root.columnconfigure(0, weight=1)
        root.rowconfigure(4, weight=1)

        header = ttk.Frame(root)
        header.grid(row=0, column=0, sticky="ew", pady=(0, 18))
        header.columnconfigure(0, weight=1)
        ttk.Label(
            header, text="DRU Firmware Sender", style="Title.TLabel"
        ).grid(row=0, column=0, sticky="w")
        ttk.Label(
            header,
            text=(
                f"IEC 62056-21 optical transfer   |   {INITIAL_BAUD} baud 7E1 "
                f"opening   |   8N1 data session"
            ),
            style="Subtitle.TLabel",
        ).grid(row=1, column=0, sticky="w", pady=(3, 0))

        status_area = ttk.Frame(
            header, style="SurfaceAlt.TFrame", padding=(14, 9, 18, 9)
        )
        status_area.grid(row=0, column=1, rowspan=2, sticky="e")
        self.status_indicator = PulseIndicator(
            status_area, self.COLORS["surface_alt"]
        )
        self.status_indicator.grid(row=0, column=0, rowspan=2, padx=(0, 7))
        ttk.Label(
            status_area, textvariable=self.status_var, style="Status.TLabel"
        ).grid(row=0, column=1, sticky="w")
        self.status_detail = ttk.Label(
            status_area, text="No active transfer", style="StatusDetail.TLabel"
        )
        self.status_detail.grid(row=1, column=1, sticky="w")

        setup = ttk.Frame(root)
        setup.grid(row=1, column=0, sticky="ew", pady=(0, 10))
        setup.columnconfigure(0, weight=1, uniform="setup")
        setup.columnconfigure(1, weight=1, uniform="setup")

        connection = ttk.LabelFrame(setup, text="Connection", padding=(16, 13))
        connection.grid(row=0, column=0, sticky="nsew", padx=(0, 6))
        connection.columnconfigure(0, weight=1)

        ttk.Label(connection, text="Serial port", style="Muted.TLabel").grid(
            row=0, column=0, columnspan=2, sticky="w", pady=(0, 5)
        )
        self.port_combo = ttk.Combobox(
            connection,
            textvariable=self.port_var,
            state="readonly",
        )
        self.port_combo.grid(row=1, column=0, sticky="ew")
        self.refresh_btn = ModernButton(
            connection,
            text="Refresh ports",
            command=self.refresh_ports,
            colors=self.COLORS,
            width=112,
            height=36,
        )
        self.refresh_btn.grid(row=1, column=1, padx=(9, 0))

        ttk.Label(connection, text="Transfer baud", style="Muted.TLabel").grid(
            row=2, column=0, columnspan=2, sticky="w", pady=(12, 5)
        )
        self.baud_combo = ttk.Combobox(
            connection,
            textvariable=self.baud_var,
            values=[str(rate) for rate in BAUD_CODES],
            state="readonly",
        )
        self.baud_combo.grid(row=3, column=0, columnspan=2, sticky="ew")

        firmware = ttk.LabelFrame(setup, text="Firmware", padding=(16, 13))
        firmware.grid(row=0, column=1, sticky="nsew", padx=(6, 0))
        firmware.columnconfigure(0, weight=1)

        mode_row = ttk.Frame(firmware, style="Surface.TFrame")
        mode_row.grid(row=0, column=0, sticky="ew")
        mode_row.columnconfigure(1, weight=1)
        ttk.Label(mode_row, text="Target", style="Muted.TLabel").grid(
            row=0, column=0, sticky="w", padx=(0, 10)
        )
        segment = ttk.Frame(mode_row, style="Surface.TFrame")
        segment.grid(row=0, column=1, sticky="w")
        self.firmware_type_buttons = []
        for index, firmware_type in enumerate(FIRMWARE_COMMANDS):
            button = ttk.Radiobutton(
                segment,
                text=firmware_type,
                value=firmware_type,
                variable=self.firmware_type_var,
                command=self._on_firmware_type_change,
                style="Segment.TRadiobutton",
            )
            button.grid(row=0, column=index, padx=(0 if index == 0 else 4, 0))
            self.firmware_type_buttons.append(button)
        ttk.Label(
            mode_row, textvariable=self.command_var, style="Command.TLabel"
        ).grid(row=0, column=2, sticky="e", padx=(12, 0))

        ttk.Label(firmware, text="Binary image", style="Muted.TLabel").grid(
            row=1, column=0, sticky="w", pady=(12, 5)
        )
        file_row = ttk.Frame(firmware, style="Surface.TFrame")
        file_row.grid(row=2, column=0, sticky="ew")
        file_row.columnconfigure(0, weight=1)
        self.file_entry = ttk.Entry(
            file_row, textvariable=self.file_var, state="readonly"
        )
        self.file_entry.grid(row=0, column=0, sticky="ew")
        self.browse_btn = ModernButton(
            file_row,
            text="Choose BIN",
            command=self.browse_file,
            colors=self.COLORS,
            width=108,
            height=36,
        )
        self.browse_btn.grid(row=0, column=1, padx=(9, 0))

        metrics_frame = ttk.Frame(firmware, style="Surface.TFrame")
        metrics_frame.grid(row=3, column=0, sticky="ew", pady=(12, 0))
        metrics = (
            ("BIN size", self.file_size_var),
            ("Data packets", self.packet_count_var),
            ("CRC Byte", self.crc_var),
        )
        for index, (label, variable) in enumerate(metrics):
            metrics_frame.columnconfigure(index, weight=1)
            metric = ttk.Frame(metrics_frame, style="Surface.TFrame")
            metric.grid(row=0, column=index, sticky="w", padx=(0 if index == 0 else 16, 8))
            ttk.Label(metric, text=label, style="Muted.TLabel").grid(
                row=0, column=0, sticky="w"
            )
            ttk.Label(metric, textvariable=variable, style="Metric.TLabel").grid(
                row=1,
                column=0,
                sticky="w",
                pady=(2, 0),
            )

        self.stage_tracker = StageTimeline(root, self.STAGES, self.COLORS)
        self.stage_tracker.grid(row=2, column=0, sticky="ew", pady=(1, 8))

        action = ttk.LabelFrame(root, text="Transfer", padding=(16, 11))
        action.grid(row=3, column=0, sticky="ew", pady=(0, 12))
        action.columnconfigure(0, weight=1)
        top_line = ttk.Frame(action, style="Surface.TFrame")
        top_line.grid(row=0, column=0, columnspan=3, sticky="ew", pady=(0, 8))
        top_line.columnconfigure(0, weight=1)
        self.progress_status = ttk.Label(
            top_line, text="Ready", style="ActionTitle.TLabel"
        )
        self.progress_status.grid(row=0, column=0, sticky="w")
        ttk.Label(
            top_line, textvariable=self.progress_text_var, style="ActionTitle.TLabel"
        ).grid(
            row=0, column=1, sticky="e"
        )
        self.progress = SmoothProgress(
            action,
            track_color="#DFE7EA",
            fill_color=self.COLORS["accent"],
            shimmer_color="#4CB7BE",
            canvas_color=self.COLORS["surface"],
        )
        self.progress.grid(row=1, column=0, sticky="ew", padx=(0, 14))
        self.cancel_btn = ModernButton(
            action,
            text="Cancel",
            command=self.cancel_upload,
            colors=self.COLORS,
            kind="danger",
            width=110,
            height=42,
        )
        self.cancel_btn.configure(state="disabled")
        self.cancel_btn.grid(row=1, column=1, padx=(0, 8))
        self.run_btn = ModernButton(
            action,
            text="Send firmware",
            command=self.start_flash_thread,
            colors=self.COLORS,
            kind="primary",
            width=132,
            height=42,
        )
        self.run_btn.grid(row=1, column=2)

        log_frame = ttk.LabelFrame(root, text="Transfer activity", padding=(12, 9))
        log_frame.grid(row=4, column=0, sticky="nsew")
        log_frame.columnconfigure(0, weight=1)
        log_frame.rowconfigure(1, weight=1)

        log_tools = ttk.Frame(log_frame, style="Surface.TFrame")
        log_tools.grid(row=0, column=0, columnspan=2, sticky="ew", pady=(0, 7))
        log_tools.columnconfigure(0, weight=1)
        ttk.Checkbutton(
            log_tools, text="Show frame bytes", variable=self.verbose_var
        ).grid(row=0, column=0, sticky="w")
        self.save_log_btn = ModernButton(
            log_tools,
            text="Save log",
            command=self.save_log,
            colors=self.COLORS,
            width=96,
            height=34,
        )
        self.save_log_btn.grid(row=0, column=1, padx=(8, 0))
        self.clear_log_btn = ModernButton(
            log_tools,
            text="Clear",
            command=self.clear_log,
            colors=self.COLORS,
            width=86,
            height=34,
        )
        self.clear_log_btn.grid(row=0, column=2, padx=(8, 0))

        self.log_text = tk.Text(
            log_frame,
            wrap="none",
            bg=self.COLORS["log"],
            fg="#DCE5EA",
            insertbackground="#FFFFFF",
            selectbackground="#28556A",
            relief="flat",
            borderwidth=0,
            padx=12,
            pady=10,
            font=("Cascadia Mono", 9),
            state="disabled",
        )
        self.log_text.grid(row=1, column=0, sticky="nsew")
        scroll_y = ttk.Scrollbar(
            log_frame, orient="vertical", command=self.log_text.yview
        )
        scroll_y.grid(row=1, column=1, sticky="ns")
        self.log_text.configure(yscrollcommand=scroll_y.set)
        self.log_text.tag_configure("tx", foreground="#51C2B4")
        self.log_text.tag_configure("rx", foreground="#F2B66D")
        self.log_text.tag_configure("info", foreground="#8BC5E3")
        self.log_text.tag_configure("success", foreground="#78D6A3")
        self.log_text.tag_configure("warning", foreground="#F2C56B")
        self.log_text.tag_configure("error", foreground="#FF8787")
        self.log_text.tag_configure("dump", foreground="#82919B")

    def _on_firmware_type_change(self):
        firmware_type = self.firmware_type_var.get()
        command = FIRMWARE_COMMANDS.get(firmware_type)
        if command is not None:
            self.command_var.set(f"CMD {command}  |  0x{command:02X}")

    def _fade_in(self):
        try:
            current = float(self.attributes("-alpha"))
            if current >= 1.0:
                return
            self.attributes("-alpha", min(1.0, current + 0.08))
            self.after(20, self._fade_in)
        except tk.TclError:
            return

    def _post(self, event_name, *values):
        self.ui_events.put((event_name, values))

    def _process_ui_events(self):
        try:
            while True:
                event_name, values = self.ui_events.get_nowait()
                if event_name == "log":
                    self._append_log(*values)
                elif event_name == "packet":
                    self._append_packet(*values)
                elif event_name == "status":
                    self._set_status(*values)
                elif event_name == "stage":
                    self._set_stage(*values)
                elif event_name == "progress":
                    self._set_progress(*values)
                elif event_name == "finished":
                    self._finish_transfer(*values)
        except queue.Empty:
            pass

        try:
            self.after(40, self._process_ui_events)
        except tk.TclError:
            pass

    def _append_log(self, message, tag="info"):
        timestamp = time.strftime("%H:%M:%S")
        self.log_text.configure(state="normal")
        self.log_text.insert(tk.END, f"[{timestamp}] {message}\n", tag)
        self.log_text.see(tk.END)
        self.log_text.configure(state="disabled")

    def _append_packet(self, direction, data, summary):
        tag = "tx" if direction == "TX" else "rx"
        self._append_log(
            f"{direction:<2}  {summary}  ({len(data)} bytes)", tag
        )
        if self.verbose_var.get() and data:
            self._append_log(format_hex_dump(data), "dump")

    def _set_status(self, title, detail, state="running"):
        self.status_var.set(title)
        self.status_detail.configure(text=detail)
        self.status_indicator.set_state(state)
        self.progress_status.configure(text=detail)

    def _set_stage(self, active_index, error=False):
        self.stage_tracker.set_stage(active_index, error)

    def _set_progress(self, value, text):
        self.progress.set_value(value)
        self.progress_text_var.set(text)

    def _set_controls_running(self, running):
        normal_state = "disabled" if running else "normal"
        combo_state = "disabled" if running else "readonly"
        self.run_btn.configure(state=normal_state)
        self.browse_btn.configure(state=normal_state)
        self.refresh_btn.configure(state=normal_state)
        self.port_combo.configure(state=combo_state)
        self.baud_combo.configure(state=combo_state)
        for button in self.firmware_type_buttons:
            button.configure(state=normal_state)
        self.cancel_btn.configure(state="normal" if running else "disabled")
        self.progress.set_active(running)

    def _finish_transfer(self, outcome, title, message, stage_index):
        self._set_controls_running(False)
        self.worker = None

        if outcome == "success":
            self.stage_tracker.set_complete()
            self._set_status("Transfer complete", message, "success")
            self.progress.set_value(100, immediate=True)
            self.progress_text_var.set("100%")
            messagebox.showinfo(title, message, parent=self)
        elif outcome == "cancelled":
            self._set_stage(stage_index, error=True)
            self._set_status("Transfer cancelled", message, "warning")
        elif outcome == "rejected":
            self._set_stage(stage_index, error=True)
            self._set_status("IEC rejected transfer", message, "error")
            messagebox.showerror(title, message, parent=self)
        else:
            self._set_stage(stage_index, error=True)
            self._set_status("Transfer failed", message, "error")
            messagebox.showerror(title, message, parent=self)

    def refresh_ports(self):
        current = self.port_var.get()
        ports = [item.device for item in serial.tools.list_ports.comports()]
        choices = [VIRTUAL_PORT] + ports
        self.port_combo["values"] = choices
        if current in choices:
            self.port_var.set(current)
        elif "COM20" in ports:
            self.port_var.set("COM20")
        else:
            self.port_var.set(VIRTUAL_PORT)

    def browse_file(self):
        path = filedialog.askopenfilename(
            parent=self,
            title="Select firmware",
            filetypes=(("Binary firmware", "*.bin"), ("All files", "*.*")),
        )
        if not path:
            return

        try:
            with open(path, "rb") as bin_f:
                data = bin_f.read()
            plan = make_transfer_plan(data)
        except (OSError, ValueError) as error:
            messagebox.showerror("Invalid firmware", str(error), parent=self)
            return

        self.file_var.set(path)
        self.file_size_var.set(format_bytes(plan.file_size))
        self.packet_count_var.set(f"{plan.block_count:,}")
        self.crc_var.set(plan.crc_hex)
        self._set_status("Firmware ready", os.path.basename(path), "idle")

    def save_log(self):
        path = filedialog.asksaveasfilename(
            parent=self,
            title="Save transfer log",
            defaultextension=".txt",
            filetypes=(("Text file", "*.txt"), ("All files", "*.*")),
        )
        if not path:
            return
        try:
            with open(path, "w", encoding="utf-8") as output:
                output.write(self.log_text.get("1.0", tk.END))
        except OSError as error:
            messagebox.showerror("Save failed", str(error), parent=self)

    def clear_log(self):
        self.log_text.configure(state="normal")
        self.log_text.delete("1.0", tk.END)
        self.log_text.configure(state="disabled")

    def cancel_upload(self):
        if self.worker and self.worker.is_alive():
            self.cancel_requested.set()
            self.cancel_btn.configure(state="disabled")
            self._set_status("Cancelling", "Waiting for serial operation", "warning")
            self._append_log("Cancellation requested by operator", "warning")

    def start_flash_thread(self):
        port = self.port_var.get().strip()
        file_path = self.file_var.get().strip()
        firmware_type = self.firmware_type_var.get().strip()

        if not port:
            messagebox.showerror("Missing port", "Select a serial port.", parent=self)
            return
        if not file_path or not os.path.isfile(file_path):
            messagebox.showerror(
                "Missing firmware", "Choose a valid BIN file.", parent=self
            )
            return

        try:
            with open(file_path, "rb") as bin_f:
                raw_data = bin_f.read()
            command = FIRMWARE_COMMANDS[firmware_type]
            baudrate = int(self.baud_var.get())
            plan = make_transfer_plan(raw_data)
        except KeyError:
            messagebox.showerror(
                "Invalid settings", "Select PLC or RF firmware.", parent=self
            )
            return
        except (OSError, ValueError) as error:
            messagebox.showerror("Invalid settings", str(error), parent=self)
            return

        if port != VIRTUAL_PORT:
            proceed = messagebox.askokcancel(
                "Start firmware transfer",
                "Keep the target powered and the serial connection attached until "
                "the transfer completes.",
                parent=self,
            )
            if not proceed:
                return

        self.cancel_requested.clear()
        self.progress.set_value(0, immediate=True)
        self.progress_text_var.set("0%")
        self._set_stage(0)
        self._set_controls_running(True)
        self._set_status("Connecting", f"Opening {port}", "running")
        self._append_log(
            f"Selected {os.path.basename(file_path)}: {plan.file_size:,} BIN bytes, "
            f"{plan.block_count:,} data packets, CRC byte: {plan.crc_hex}, "
            f"{firmware_type} command {command} (0x{command:02X})",
            "info",
        )

        self.worker = threading.Thread(
            target=self.run_process,
            args=(port, baudrate, file_path, plan, firmware_type, command),
            daemon=True,
        )
        self.worker.start()

    def _check_cancelled(self):
        if self.cancel_requested.is_set():
            raise TransferCancelled("The transfer was cancelled by the operator.")

    def _sleep(self, seconds):
        if self.cancel_requested.wait(seconds):
            self._check_cancelled()

    def _open_serial(self, port):
        if port == VIRTUAL_PORT:
            self._post("log", "Using virtual IEC target", "info")
            return VirtualSerialDevice()

        serial_port = serial.Serial(
            port=port,
            baudrate=INITIAL_BAUD,
            bytesize=serial.SEVENBITS,
            parity=serial.PARITY_EVEN,
            stopbits=serial.STOPBITS_ONE,
            timeout=0.05,
            write_timeout=2.0,
            xonxoff=False,
            rtscts=False,
            dsrdtr=False,
        )
        serial_port.dtr = False
        serial_port.rts = False
        return serial_port

    def _clear_input(self, serial_port):
        reset = getattr(serial_port, "reset_input_buffer", None)
        if callable(reset):
            reset()

    def _write(self, serial_port, data, summary):
        self._check_cancelled()
        serial_port.write(data)
        flush = getattr(serial_port, "flush", None)
        if callable(flush):
            flush()
        self._post("packet", "TX", data, summary)

    def _wait_for_tx_drain(self, serial_port, byte_count, is_virtual):
        if is_virtual:
            return

        deadline = time.monotonic() + 2.0
        while getattr(serial_port, "out_waiting", 0) and time.monotonic() < deadline:
            self._sleep(0.005)

        wire_time = (
            byte_count * HANDSHAKE_BITS_PER_CHARACTER / float(INITIAL_BAUD)
        )
        self._sleep(wire_time + BAUD_CHANGE_DELAY_SECONDS)

    def _read_metadata(self, serial_port, is_virtual):
        deadline = time.monotonic() + METADATA_TIMEOUT_SECONDS
        quiet_time = 0.01 if is_virtual else METADATA_QUIET_SECONDS
        received = bytearray()
        last_received = None

        while time.monotonic() < deadline:
            self._check_cancelled()
            waiting = getattr(serial_port, "in_waiting", 0)
            if waiting:
                received.extend(serial_port.read(waiting))
                last_received = time.monotonic()
            elif last_received is not None:
                if time.monotonic() - last_received >= quiet_time:
                    break
            self._sleep(0.005 if is_virtual else 0.01)

        return bytes(received)

    def _read_target(
        self,
        serial_port,
        timeout,
        markers=(),
        framed=False,
        line_prefix=None,
    ):
        deadline = time.monotonic() + timeout
        received = bytearray()

        while time.monotonic() < deadline:
            self._check_cancelled()
            waiting = getattr(serial_port, "in_waiting", 0)
            if waiting:
                received.extend(serial_port.read(waiting))

                if any(marker in received for marker in markers):
                    return bytes(received)

                if line_prefix is not None:
                    start = received.find(line_prefix)
                    if start >= 0:
                        end = received.find(b"\r\n", start + len(line_prefix))
                        if end >= 0:
                            return bytes(received[start:end + 2])

                if framed:
                    start = received.find(bytes([STX]))
                    if start >= 0 and len(received) >= start + 3:
                        frame_size = (
                            int.from_bytes(
                                received[start + 1:start + 3],
                                byteorder=LENGTH_BYTEORDER,
                            )
                            + LENGTH_OVERHEAD
                        )
                        if len(received) >= start + frame_size:
                            return bytes(received[start:start + frame_size])
            self._sleep(0.01)

        return bytes(received)

    def _read_iec_response(self, serial_port):
        return self._read_target(
            serial_port,
            ACK_TIMEOUT_SECONDS,
            markers=RESPONSE_MARKERS,
        )

    def _require_ack(self, response, operation):
        response_type = classify_iec_response(response)
        if response_type == IECResponse.ACK:
            return
        if response_type == IECResponse.REJECT:
            raise IECRejectError(
                f"The IEC target rejected {operation} (06 52 0D 0A)."
            )
        if response_type == IECResponse.NAK:
            raise IECRejectError(
                f"The target returned a legacy NAK for {operation} (15 4E 0D 0A)."
            )
        if response_type == IECResponse.TIMEOUT:
            raise TransferError(
                f"No ACK was received for {operation} within "
                f"{ACK_TIMEOUT_SECONDS:.1f} seconds."
            )
        raise TransferError(f"Unexpected IEC response while waiting for {operation}.")

    def _identify(self, serial_port):
        for attempt in range(1, 4):
            self._clear_input(serial_port)
            self._write(
                serial_port,
                IDENTIFICATION_REQUEST,
                f"Identification request {attempt}/3 at {INITIAL_BAUD} baud",
            )
            response = self._read_target(
                serial_port,
                IDENTIFICATION_TIMEOUT_SECONDS,
                line_prefix=b"/SEE",
            )
            if response:
                self._post("packet", "RX", response, "Identification response")
            if response.startswith(b"/") and response.endswith(b"\r\n"):
                return
        raise TransferError("The target did not return a valid identification response.")

    def _negotiate(self, serial_port, baudrate, is_virtual):
        self._post("stage", 1)
        self._post(
            "status",
            "Negotiating",
            f"Changing to {baudrate} baud, 8N1",
            "running",
        )
        command = build_baud_command(baudrate)
        self._clear_input(serial_port)
        self._write(serial_port, command, f"Select {baudrate} baud, mode 7")
        self._wait_for_tx_drain(serial_port, len(command), is_virtual)

        serial_port.baudrate = baudrate
        serial_port.bytesize = serial.EIGHTBITS
        serial_port.parity = serial.PARITY_NONE
        serial_port.stopbits = serial.STOPBITS_ONE
        if not is_virtual:
            serial_port.dtr = False
            serial_port.rts = False
            self._clear_input(serial_port)
        self._post("log", f"Serial format changed to {baudrate} baud, 8N1", "success")

        metadata = self._read_metadata(serial_port, is_virtual)
        if not metadata:
            raise TransferError("The target did not send its metadata frame.")
        self._post("packet", "RX", metadata, "Target metadata")
        valid, reason = validate_protocol_frame(metadata)
        if not valid:
            received_hex = metadata.hex(" ").upper()
            self._post("log", f"Raw metadata bytes: {received_hex}", "warning")
            self._post(
                "log",
                f"Metadata uses a legacy format ({reason}); continuing to the "
                "initialization ACK check.",
                "warning",
            )

    def _initialize(self, serial_port):
        self._post("stage", 2)
        self._post(
            "status",
            "Initializing",
            "Waiting for IEC initialization ACK",
            "running",
        )
        frame = build_init_frame()
        self._clear_input(serial_port)
        self._write(serial_port, frame, "Initialization frame")
        response = self._read_iec_response(serial_port)
        if response:
            self._post("packet", "RX", response, "Initialization response")
        self._require_ack(response, "the initialization frame")

    def _send_blocks(self, serial_port, firmware_data, plan, firmware_type, command):
        self._post("stage", 3)
        for sequence in range(plan.block_count):
            self._check_cancelled()
            offset = sequence * DATA_BLOCK_SIZE
            payload = firmware_data[offset:offset + DATA_BLOCK_SIZE]
            frame = build_data_frame(sequence, payload, command)
            valid, reason = validate_protocol_frame(frame)
            if not valid:
                raise TransferError(
                    f"Generated {firmware_type} packet {sequence + 1} "
                    f"is invalid: {reason}."
                )

            self._post(
                "status",
                "Transferring",
                f"Packet {sequence + 1} of {plan.block_count}",
                "running",
            )
            self._clear_input(serial_port)
            self._write(
                serial_port,
                frame,
                (
                    f"{firmware_type} packet {sequence + 1}/{plan.block_count}, "
                    f"command 0x{command:02X}, "
                    f"sequence 0x{sequence:04X}, payload {len(payload)} bytes"
                ),
            )
            response = self._read_iec_response(serial_port)
            if response:
                self._post(
                    "packet", "RX", response, f"Packet {sequence + 1} response"
                )
            self._require_ack(
                response, f"{firmware_type} packet {sequence + 1}"
            )

            progress = ((sequence + 1) / plan.block_count) * 100.0
            self._post(
                "progress",
                progress,
                f"{progress:.0f}%  ({sequence + 1}/{plan.block_count})",
            )

    def _send_integrity_crc(self, serial_port, plan, firmware_type, command):
        """Send the calculated integrity CRC byte following block transfer."""
        self._check_cancelled()
        self._post("status", "Verifying", f"Sending Integrity CRC ({plan.crc_hex})", "running")
        self._post("log", f"Transmitting BIN Integrity CRC Byte: {plan.crc_hex}", "info")

        # CRC payload formatted as single byte (uint8_t)
        crc_payload = bytes([plan.crc])

        # Send with sequence ID following the last data block
        crc_frame = build_data_frame(plan.block_count, crc_payload, command)
        self._clear_input(serial_port)
        self._write(
            serial_port,
            crc_frame,
            f"{firmware_type} Integrity CRC frame ({plan.crc_hex})",
        )
        response = self._read_iec_response(serial_port)
        if response:
            self._post("packet", "RX", response, "CRC verification response")
        self._require_ack(response, "the firmware CRC verification frame")
        self._post("log", f"Firmware CRC acknowledged by target: {plan.crc_hex}", "success")

    def _terminate(self, serial_port, baudrate, firmware_type, command):
        self._post("stage", 4)
        self._post("status", "Finishing", "Sending termination frame", "running")
        frame = build_termination_frame(command)
        self._write(
            serial_port,
            frame,
            f"{firmware_type} termination, command 0x{command:02X}, "
            f"at {baudrate} baud",
        )

    def run_process(
        self, port, baudrate, bin_path, plan, firmware_type, command
    ):
        serial_port = None
        active_stage = 0
        try:
            with open(bin_path, "rb") as firmware_file:
                firmware_data = firmware_file.read()
            if len(firmware_data) != plan.file_size:
                raise TransferError("The BIN file changed after it was selected.")

            serial_port = self._open_serial(port)
            self._post(
                "log",
                f"Opened {port} at {INITIAL_BAUD} baud, 7E1",
                "info",
            )
            self._identify(serial_port)

            active_stage = 1
            self._negotiate(serial_port, baudrate, port == VIRTUAL_PORT)

            active_stage = 2
            self._initialize(serial_port)

            active_stage = 3
            self._send_blocks(
                serial_port, firmware_data, plan, firmware_type, command
            )

            # Send calculated integrity CRC byte
            self._send_integrity_crc(
                serial_port, plan, firmware_type, command
            )

            active_stage = 4
            self._terminate(serial_port, baudrate, firmware_type, command)
            self._post(
                "log",
                f"Transfer completed successfully: {plan.block_count} packets (CRC {plan.crc_hex})",
                "success",
            )
            self._post(
                "finished",
                "success",
                "Transfer complete",
                f"Sent {plan.file_size:,} firmware bytes in "
                f"{plan.block_count:,} packets.\nIntegrity CRC: {plan.crc_hex}",
                4,
            )
        except TransferCancelled as error:
            self._post("log", str(error), "warning")
            self._post(
                "finished", "cancelled", "Transfer cancelled", str(error), active_stage
            )
        except IECRejectError as error:
            self._post("log", str(error), "error")
            self._post(
                "finished", "rejected", "IEC rejected transfer", str(error), active_stage
            )
        except (TransferError, serial.SerialException, OSError, ValueError) as error:
            self._post("log", f"Transfer failed: {error}", "error")
            self._post(
                "finished", "error", "Transfer failed", str(error), active_stage
            )
        except Exception as error:
            self._post("log", f"Unexpected error: {error}", "error")
            self._post(
                "finished", "error", "Unexpected error", str(error), active_stage
            )
        finally:
            if serial_port is not None:
                try:
                    serial_port.close()
                except Exception:
                    pass

    def _close_window(self):
        if self.worker and self.worker.is_alive():
            close_now = messagebox.askyesno(
                "Transfer in progress",
                "Cancel the transfer and close the application?",
                parent=self,
            )
            if not close_now:
                return
            self.cancel_requested.set()
        self.destroy()


def main():
    app = DRUFlasherApp()
    try:
        app.attributes("-alpha", 0.72)
    except tk.TclError:
        pass
    app.mainloop()


if __name__ == "__main__":
    main()