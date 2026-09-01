# configuration/config.py
# ============================================================
# Protocol constants
# ============================================================

STX = 0x02

# Final five bytes of a complete frame:
#   21 0D 0A 03 BCC
TRAILER = b"\x21\x0d\x0a\x03"

LENGTH_OVERHEAD = 6
LENGTH_BYTEORDER = "little"

INITIAL_BAUD = 300

# Firmware command byte sent at frame byte index 6.
FIRMWARE_COMMANDS = {
    "PLC": 0xD4,  # 212 decimal
    "RF": 0xD3,   # 211 decimal
}
DEFAULT_FIRMWARE_TYPE = "PLC"

# IEC-style baud-rate selector used by the negotiation command.
BAUD_CODES = {
    300: "0",
    600: "1",
    1200: "2",
    2400: "3",
    4800: "4",
    9600: "5",
    19200: "6",
    115200: "7",
    28800: "8",
    57600: "9",
    14400: "A",
    38400: "B",
}

# Reverse mapping: character code to baud rate integer
CODE_TO_BAUD = {code: rate for rate, code in BAUD_CODES.items()}
