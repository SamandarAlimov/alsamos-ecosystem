# AlsamOS engineering agent instructions
- Samhus is AlsamOS's intended primary language. Do not falsely describe current C/assembly as Samhus kernel code.
- Read the upstream Samhus AGENTS.md and docs before making language assumptions. .sahu is provisional.
- The 32-bit Multiboot bootloader target is an experimental scaffold, not a finalized architecture.
- Keep all changes isolated from Superapp, backend, production data and credentials.
- Tests must distinguish ELF structural checks, QEMU boot/runtime checks and Samhus host-only checks.
- Avoid privileged filesystem/network behavior; prototype on QEMU before hardware.
- No licensing decision has been made; do not add one without approval.
