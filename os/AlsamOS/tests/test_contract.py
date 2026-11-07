from pathlib import Path
import subprocess
import unittest
import sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/"tools"))
from verify_multiboot import verify_elf

class BootKernelTests(unittest.TestCase):
    def test_elf_multiboot_header(self):
        self.assertLess(verify_elf(ROOT/"build"/"alsamos.elf"),8192)
    def test_no_host_interpreter(self):
        proc=subprocess.run(["readelf","-l",str(ROOT/"build"/"alsamos.elf")],
                            check=True,capture_output=True,text=True)
        self.assertNotIn("INTERP",proc.stdout)
    def test_symbols(self):
        proc=subprocess.run(["nm",str(ROOT/"build"/"alsamos.elf")],
                            check=True,capture_output=True,text=True)
        self.assertIn(" kernel_main",proc.stdout)
        self.assertIn(" _start",proc.stdout)
    def test_host_samhus_policy_source(self):
        source=(ROOT/"samhus"/"boot_policy.sahu").read_text()
        self.assertIn("fn boot_policy(memory_mib: Int, storage_ready: Bool) -> Int",source)
        for value in ("return 0","return 10","return 20"):
            self.assertIn(value,source)
