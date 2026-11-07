.RECIPEPREFIX := >
CC ?= gcc
LD ?= ld
AS ?= as
PYTHON ?= python3
QEMU ?= qemu-system-i386
BUILD := build
CFLAGS := -std=c11 -O2 -Wall -Wextra -Werror -pedantic -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -fno-builtin -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-omit-frame-pointer
OBJS := $(BUILD)/multiboot.o $(BUILD)/kmain.o $(BUILD)/console.o $(BUILD)/shell.o

.PHONY: all clean run run-serial test test-emulator samhus-check
all: $(BUILD)/alsamos.elf

$(BUILD):
>mkdir -p $(BUILD)

$(BUILD)/multiboot.o: boot/multiboot.S | $(BUILD)
>$(AS) --32 $< -o $@

$(BUILD)/%.o: kernel/%.c kernel/console.h kernel/io.h kernel/shell.h | $(BUILD)
>$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/alsamos.elf: $(OBJS) kernel/linker.ld
>$(LD) -m elf_i386 -nostdlib --build-id=none -z max-page-size=0x1000 -T kernel/linker.ld $(OBJS) -o $@
>$(PYTHON) tools/verify_multiboot.py $@

run: all
>$(QEMU) -kernel $(BUILD)/alsamos.elf -m 128M -serial stdio -monitor none

run-serial: all
>$(QEMU) -kernel $(BUILD)/alsamos.elf -m 128M -display none -serial stdio -monitor none

test: all
>$(PYTHON) -m unittest discover -s tests -v

test-emulator: all
>$(PYTHON) tools/qemu_smoke.py

samhus-check:
>$(PYTHON) tools/test_samhus.py

clean:
>rm -rf $(BUILD)
