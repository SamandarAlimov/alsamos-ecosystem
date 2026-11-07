"""Verify Samhus hosted policy against VM and native compiler backends."""
from pathlib import Path
import subprocess
import sys
ROOT=Path(__file__).resolve().parents[1]
SOURCE=ROOT/"samhus"/"boot_policy.sahu"
NATIVE=ROOT/"build"/"alsamos-policy"
EXPECTED="0\n10\n20\n"
def run(*args):
    return subprocess.run(args,check=True,cwd=ROOT,text=True,capture_output=True).stdout
vm=run(sys.executable,"-m","samhus","run",str(SOURCE),"--backend","vm","--typecheck")
assert vm==EXPECTED, f"Samhus VM mismatch: {vm!r}"
run(sys.executable,"-m","samhus","native-ir",str(SOURCE),"-o",str(NATIVE))
native=run(str(NATIVE))
assert native==EXPECTED, f"Samhus native mismatch: {native!r}"
print("Samhus VM/native host policy passed")
