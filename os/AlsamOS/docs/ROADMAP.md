# AlsamOS engineering roadmap (proposed; not final decisions)

## Milestone 0 — proof-of-boot scaffold
- Experimental i386 ELF32 Multiboot v1 kernel with 16 KiB startup stack.
- Freestanding C VGA/COM1 console and polled PS/2 + serial shell.
- Samhus hosted boot policy module with experimental VM/native parity test.
- Structural ELF test; CI also attempts QEMU boot and shell verification.

## Milestone 1 — Samhus freestanding subset (critical path)
- Prototype small typed Samhus kernel-ready object-code compiler target.
- Define safe, reviewed FFI/unsafe and calling convention boundaries.
- No libc/Python/runtime dependencies in emitted linkable code.
- Validate instruction behavior, stack and integer semantics in emulator.
- Only then port kernel routines into native Samhus.

## Milestones 2–4 — real OS
- GDT/IDT and exceptions; memory map/paging/allocator; timer/scheduler.
- Protected processes and capability-based syscalls; storage/VFS and drivers.
- Sandboxed Samhus userland, process loader, managed permissions and updates.
- Desktop compositor and application framework, then hardware portability.

**Not done**: native Samhus execution inside OS, filesystem, networking,
process isolation, interrupt handlers, UEFI, full GUI, security audit,
or physical-device hardware support. x86-32 and C are temporary experiments.
