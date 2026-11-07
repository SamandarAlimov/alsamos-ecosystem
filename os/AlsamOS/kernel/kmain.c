#include <stdint.h>
#include "console.h"
#include "shell.h"
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002u
#define MULTIBOOT_INFO_MEMORY 1u
typedef struct { uint32_t flags,mem_lower,mem_upper; } multiboot_info_prefix;

void kernel_main(uint32_t magic, uint32_t boot_info_addr) {
    console_init();
    console_write("======================================\n");
    console_write("             AlsamOS                  \n");
    console_write("        Experimental kernel           \n");
    console_write("======================================\n");
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        console_write("FATAL: invalid Multiboot boot magic: ");
        console_hex(magic); console_write("\n");
        for (;;) { __asm__ volatile("cli; hlt"); }
    }
    console_write("[OK] Multiboot 1 handoff verified\n");
    console_write("[OK] VGA and COM1 consoles initialized\n");
    console_write("[INFO] Samhus userland prototype is host-only for now\n\n");
    uint32_t memory_kib=0;
    int has_memory=0;
    if (boot_info_addr) {
        const multiboot_info_prefix *info=
            (const multiboot_info_prefix *)(uintptr_t)boot_info_addr;
        if (info->flags & MULTIBOOT_INFO_MEMORY) {
            memory_kib=info->mem_upper; has_memory=1;
        }
    }
    shell_init(memory_kib,has_memory);
    for (;;) shell_poll();
}
