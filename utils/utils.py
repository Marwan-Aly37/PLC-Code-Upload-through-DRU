# utils/utils.py
from configuration.config import (
    STX,
    TRAILER,
    LENGTH_OVERHEAD,
    LENGTH_BYTEORDER,
    BAUD_CODES,
)


def build_baud_command(baudrate: int) -> bytes:
    """Build the IEC configuration-mode baud negotiation command."""
    if baudrate not in BAUD_CODES:
        raise ValueError(
            f"Unsupported negotiated baud rate: {baudrate}. "
            f"Supported rates: {', '.join(map(str, BAUD_CODES))}"
        )

    return (
        b"\x06"
        + b"0"
        + BAUD_CODES[baudrate].encode("ascii")
        + b"7\r\n"
    )


# ============================================================
# BCC
# ============================================================

def calculate_bcc(data: bytes) -> int:
    """Return the firmware XOR BCC, limited to seven bits."""
    bcc = 0
    for byte in data:
        bcc ^= byte
    return bcc & 0x7F


# ============================================================
# Protocol length
# ============================================================

def calculate_protocol_length(body_without_length: bytes) -> int:
    """
    Calculate the value placed in the two-byte length field.
    length = total_frame_length - 6
    """
    total_frame_length = (
        1                       # STX
        + 2                     # length
        + len(body_without_length)
        + 1                     # BCC
    )

    protocol_length = (
        total_frame_length
        - LENGTH_OVERHEAD
    )

    if not 0 <= protocol_length <= 0xFFFF:
        raise ValueError(
            f"Invalid protocol length: {protocol_length}"
        )

    return protocol_length


# ============================================================
# DATA FRAME
# ============================================================

def build_data_frame(
    seq: int,
    payload_chunk: bytes,
    command: int = 0xD4,
) -> bytes:
    """Build one firmware data packet using little-endian length."""
    if not 0 <= command <= 0xFF:
        raise ValueError("The firmware command must fit in one byte.")

    stx = bytes([STX])

    header = bytes([
        0x01,
        0x00,
        0x00,
        command,
        0x05,
        seq & 0xFF,
        (seq >> 8) & 0xFF,
        0x00,
        0x00,
        0x05,
        0x08,
    ])

    body_without_length = (
        header
        + payload_chunk
        + TRAILER
    )

    protocol_length = calculate_protocol_length(
        body_without_length
    )

    length_field = protocol_length.to_bytes(
        2,
        byteorder=LENGTH_BYTEORDER
    )

    body = (
        length_field
        + body_without_length
    )

    bcc = calculate_bcc(body)

    frame = (
        stx
        + body
        + bytes([bcc])
    )

    return frame


# ============================================================
# INITIALIZATION FRAME
# ============================================================

def build_init_frame() -> bytes:
    """Build the fixed initialization frame."""
    stx = bytes([STX])

    fields = bytes([
        0x02,
        0x00,
        0x00,
        0x00,
        0x64,
        0x00,
        0x00,
        0x00,
        0x00,
        0x06,
        0x08,
        0x1A,
    ])

    body_without_length = (
        fields
        + TRAILER
    )

    protocol_length = calculate_protocol_length(
        body_without_length
    )

    length_field = protocol_length.to_bytes(
        2,
        byteorder=LENGTH_BYTEORDER
    )

    body = (
        length_field
        + body_without_length
    )

    bcc = calculate_bcc(body)

    return (
        stx
        + body
        + bytes([bcc])
    )


# ============================================================
# TERMINATION FRAME
# ============================================================

def build_termination_frame(command: int = 0xD4) -> bytes:
    """Build the end frame using the selected firmware command byte."""
    if not 0 <= command <= 0xFF:
        raise ValueError("The firmware command must fit in one byte.")

    body_without_length = b"\x00\x00\x00" + bytes([command]) + TRAILER
    protocol_length = calculate_protocol_length(body_without_length)
    length_field = protocol_length.to_bytes(
        2,
        byteorder=LENGTH_BYTEORDER,
    )
    body = length_field + body_without_length

    bcc = calculate_bcc(body)

    return (
        bytes([STX])
        + body
        + bytes([bcc])
    )


def validate_protocol_frame(frame: bytes):
    """Validate STX, length, trailer, and BCC of a complete frame."""
    if len(frame) < 10:
        return False, "frame is too short"
    if frame[0] != STX:
        return False, "STX is missing"

    protocol_length = int.from_bytes(
        frame[1:3],
        byteorder=LENGTH_BYTEORDER,
    )
    if len(frame) != protocol_length + LENGTH_OVERHEAD:
        return False, "length field does not match frame size"
    if frame[-5:-1] != TRAILER:
        return False, "frame ending bytes are incorrect"
    if calculate_bcc(frame[1:-1]) != frame[-1]:
        return False, "BCC is incorrect"
    return True, ""


# ============================================================
# HEX DUMP
# ============================================================

def format_hex_dump(
    data: bytes,
    bytes_per_line: int = 16
) -> str:
    lines = []
    for i in range(0, len(data), bytes_per_line):
        chunk = data[i : i + bytes_per_line]
        hex_part = " ".join(f"{b:02x}" for b in chunk)
        ascii_part = "".join(
            chr(b) if 32 <= b <= 126 else "." for b in chunk
        )
        lines.append(f"    {hex_part:<48}  {ascii_part}")
    return "\n".join(lines)
