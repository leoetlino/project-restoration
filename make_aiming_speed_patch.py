#!/usr/bin/env python3

import struct
import sys

target_path = sys.argv[1]
multiplier = float(sys.argv[2])

def va_to_file_offset(va):
  return va - 0x100000
with open(target_path, "rb+") as f:
  f.seek(va_to_file_offset(0x21CA14))
  f.write(struct.pack('<f', 725.0 * multiplier))

  f.seek(va_to_file_offset(0x21CA20))
  f.write(struct.pack('<f', -715.0 * multiplier))
