#include "shell.h"
#include "console.h"
#include "io.h"

static char line[80];
static uint32_t used, memory_kib;
static int has_memory, shift_down, pending_cr;
static const char keymap[128] = {
    [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',
    [0x07]='6',[0x08]='7',[0x09]='8',[0x0A]='9',[0x0B]='0',
    [0x0C]='-',[0x0D]='=',[0x10]='q',[0x11]='w',[0x12]='e',
    [0x13]='r',[0x14]='t',[0x15]='y',[0x16]='u',[0x17]='i',
    [0x18]='o',[0x19]='p',[0x1A]='[',[0x1B]=']',[0x1E]='a',
    [0x1F]='s',[0x20]='d',[0x21]='f',[0x22]='g',[0x23]='h',
    [0x24]='j',[0x25]='k',[0x26]='l',[0x27]=';',[0x28]='\'',
    [0x29]='`',[0x2B]='\\',[0x2C]='z',[0x2D]='x',[0x2E]='c',
    [0x2F]='v',[0x30]='b',[0x31]='n',[0x32]='m',[0x33]=',',
    [0x34]='.',[0x35]='/',[0x39]=' '
};
static int equals(const char *a,const char *b) {
    while (*a && *a==*b) { ++a; ++b; }
    return *a==*b;
}
static void prompt(void) { console_write("alsamos> "); }
static void execute(void) {
    line[used]='\0';
    if (equals(line,"help")) console_write("Commands: help, about, clear, mem, halt\n");
    else if (equals(line,"about")) console_write("AlsamOS pre-alpha | x86 Multiboot 1 | Samhus integration pending\n");
    else if (equals(line,"clear")) console_clear();
    else if (equals(line,"mem")) {
        if (has_memory) {
            console_write("Reported memory above 1 MiB: ");
            console_uint(memory_kib/1024u);
            console_write(" MiB (+ remainder ");
            console_uint(memory_kib%1024u);
            console_write(" KiB)\n");
        } else console_write("Bootloader memory information unavailable\n");
    } else if (equals(line,"halt")) {
        console_write("CPU halted.\n");
        for (;;) { __asm__ volatile("cli; hlt"); }
    } else if (used) console_write("Unknown command. Type help.\n");
    used=0; prompt();
}
static void accept_char(int incoming) {
    if (incoming=='\r' || incoming=='\n') {
        if (incoming=='\n' && pending_cr) { pending_cr=0; return; }
        pending_cr=incoming=='\r';
        console_putc('\n'); execute(); return;
    }
    pending_cr=0;
    if (incoming=='\b' || incoming==0x7F) {
        if (used) { --used; console_putc('\b'); }
        return;
    }
    if (incoming>=32 && incoming<=126 && used<sizeof(line)-1u) {
        line[used++]=(char)incoming;
        console_putc((char)incoming);
    }
}
static void keyboard_poll(void) {
    if (!(inb(0x64)&1)) return;
    uint8_t scan=inb(0x60);
    if (scan==0x2A || scan==0x36) { shift_down=1; return; }
    if (scan==0xAA || scan==0xB6) { shift_down=0; return; }
    if (scan&0x80) return;
    if (scan==0x1C) { accept_char('\r'); return; }
    if (scan==0x0E) { accept_char('\b'); return; }
    char c=keymap[scan];
    if (shift_down && c>='a' && c<='z') c=(char)(c-'a'+'A');
    if (c) accept_char((int)c);
}
void shell_init(uint32_t mem_upper_kib, int memory_available) {
    memory_kib=mem_upper_kib; has_memory=memory_available;
    used=0; shift_down=0; pending_cr=0;
    prompt();
}
void shell_poll(void) {
    int serial_input=console_serial_read();
    if (serial_input>=0) accept_char(serial_input);
    keyboard_poll();
    cpu_pause();
}
