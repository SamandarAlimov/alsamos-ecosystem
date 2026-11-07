# AlsamOS — experimental OS bootstrap

**Status: pre-alpha experiment, not a production-ready OS.** Samhus is the intended primary implementation language. Today's Samhus compiler lacks a freestanding target and ABI; therefore this initial bootable kernel uses temporary freestanding assembly/C. The Samhus source in `samhus/` runs on the development host, **not inside the kernel**.

## Build (Linux with gcc -m32, binutils, make, python3)

```bash
make test
make run-serial     # requires qemu-system-i386
make test-emulator  # requires qemu-system-i386; black-box serial shell checks
```

From this directory, the build produces `build/alsamos.elf` (Multiboot v1, ELF32 i386). Try the serial shell commands `help`, `about`, `mem`, `clear`, `halt`. Neither persistent storage nor networking exists. The kernel input loop polls PS/2 set-1 keyboard/COM1 and does not yet support interrupts.

## Samhus policy module (HOST-ONLY)

With the founder's current compiler from `SamandarAlimov/samhus` installed:

```bash
python -m samhus run samhus/boot_policy.sahu --backend vm --typecheck
python -m samhus native-ir samhus/boot_policy.sahu -o build/alsamos-policy
./build/alsamos-policy
python tools/test_samhus.py
```

Both backends should print `0`, `10`, `20` on separate lines. Current `.sahu` filename suffix is provisional upstream.

## Next engineering gates

1. Define a small typed freestanding Samhus subset, with no libc, printf, Python, or hosted runtime.
2. Produce linkable machine-code objects and verify ABI, stack/overflow behavior.
3. Move tested kernel routines from C into Samhus; only after that claim native Samhus kernel integration.
4. Implement IDT, memory manager, MMU, processes, capability isolation, storage/drivers, security hardening, signed updates and a GUI.

The initial x86 32-bit target and C bootstrap are reversible experiments, not founder-approved permanent OS design decisions. No open-source license has been selected.
