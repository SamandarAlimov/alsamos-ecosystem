"""Black-box QEMU Multiboot boot plus COM1 serial shell smoke."""
import os
from pathlib import Path
import select
import subprocess
import sys
import time
ROOT=Path(__file__).resolve().parents[1]
cmd=[os.environ.get("QEMU","qemu-system-i386"),"-kernel",str(ROOT/"build"/"alsamos.elf"),
     "-machine","pc","-m","128M","-display","none","-monitor","none",
     "-serial","stdio","-no-reboot","-no-shutdown"]
def test():
    p=subprocess.Popen(cmd,stdin=subprocess.PIPE,stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE,bufsize=0)
    received=b""
    def expect(fragment):
        nonlocal received
        end=time.monotonic()+15
        while fragment not in received:
            if p.poll() is not None: raise RuntimeError(f"QEMU exited: {p.returncode}, {received!r}")
            if time.monotonic()>end: raise TimeoutError(f"missing {fragment!r}: {received[-2000:]!r}")
            ready,_,_=select.select([p.stdout],[],[],0.15)
            if ready:
                chunk=os.read(p.stdout.fileno(),4096)
                if not chunk: raise RuntimeError("QEMU closed serial stdout")
                received+=chunk
    try:
        expect(b"alsamos> ")
        for command,expected in [(b"help\r",b"Commands: help, about, clear, mem, halt"),
                                 (b"about\r",b"AlsamOS pre-alpha"),
                                 (b"mem\r",b"Reported memory above 1 MiB"),
                                 (b"halt\r",b"CPU halted.")]:
            p.stdin.write(command); p.stdin.flush(); expect(expected)
            print(f"QEMU verified {command.strip()!r}")
        print("QEMU kernel and serial shell passed")
    finally:
        p.kill(); p.communicate(timeout=5)
if __name__=="__main__":
    try: test()
    except (OSError,RuntimeError,TimeoutError) as err:
        print(f"QEMU smoke failed: {err}",file=sys.stderr); sys.exit(1)
