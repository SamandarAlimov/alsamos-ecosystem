/* Multiboot v1 handoff, experimental x86-32 only. */
.set MB_MAGIC, 0x1BADB002
.set MB_FLAGS, 0x00000003
.set MB_CHECKSUM, -(MB_MAGIC + MB_FLAGS)
.section .multiboot, "a", @progbits
.align 4
.long MB_MAGIC
.long MB_FLAGS
.long MB_CHECKSUM
.section .bss, "aw", @nobits
.align 16
stack_bottom:
.skip 16384
stack_top:
.section .text, "ax", @progbits
.global _start
.type _start, @function
.extern kernel_main
_start:
    cli
    mov $stack_top, %esp
    and $-16, %esp
    push %ebx
    push %eax
    call kernel_main
halt_forever:
    cli
    hlt
    jmp halt_forever
.size _start, . - _start
.section .note.GNU-stack,"",@progbits
