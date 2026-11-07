"""Validate ELF32 i386 and Multiboot v1 header in first 8192 bytes."""
from pathlib import Path
import struct
import sys

MAGIC=0x1BADB002
def verify_elf(path: Path) -> int:
    raw=path.read_bytes()
    if raw[:4]!=b"\x7fELF" or raw[4:6]!=b"\x01\x01":
        raise ValueError("expected little-endian ELF32")
    if len(raw)<52: raise ValueError("ELF header truncated")
    machine=struct.unpack_from("<H",raw,18)[0]
    if machine!=3: raise ValueError("expected i386 e_machine=3")
    entry=struct.unpack_from("<I",raw,24)[0]
    if not entry: raise ValueError("zero ELF entry")
    for offset in range(0,min(8192,len(raw)-12)+1,4):
        magic,flags,checksum=struct.unpack_from("<III",raw,offset)
        if magic==MAGIC and (magic+flags+checksum)&0xFFFFFFFF==0:
            if flags!=3: raise ValueError("unsupported Multiboot flags")
            print(f"Multiboot v1 valid; header offset={offset}; entry={entry:#x}")
            return offset
    raise ValueError("no valid Multiboot v1 header in first 8192 bytes")
if __name__=="__main__":
    try: verify_elf(Path(sys.argv[1]))
    except (IndexError,OSError,ValueError) as err:
        print(f"Invalid kernel: {err}",file=sys.stderr)
        sys.exit(1)
