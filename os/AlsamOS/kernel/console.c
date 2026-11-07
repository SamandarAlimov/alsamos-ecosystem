#include "console.h"
#include "io.h"

#define VGA_WIDTH 80u
#define VGA_HEIGHT 25u
#define COM1 0x3F8u
static volatile uint16_t *const vga = (volatile uint16_t *)0xB8000;
static uint32_t row, col;
static uint8_t color = 0x0F;
static int serial_available;

static void serial_init(void) {
    outb(COM1+1, 0);
    outb(COM1+3, 0x80);
    outb(COM1+0, 3);
    outb(COM1+1, 0);
    outb(COM1+3, 3);
    outb(COM1+2, 0xC7);
    outb(COM1+4, 0x1E);
    outb(COM1+0, 0xAE);
    serial_available = inb(COM1+0) == 0xAE;
    outb(COM1+4, 0x0F);
}
static void serial_write(char c) {
    if (!serial_available) return;
    if (c == '\n') serial_write('\r');
    for (uint32_t timeout = 0; timeout < 100000u; ++timeout) {
        if (inb(COM1+5) & 0x20) { outb(COM1, (uint8_t)c); return; }
    }
}
static void scroll_if_needed(void) {
    if (row < VGA_HEIGHT) return;
    for (uint32_t r=1; r<VGA_HEIGHT; ++r)
        for (uint32_t c=0; c<VGA_WIDTH; ++c)
            vga[(r-1u)*VGA_WIDTH+c] = vga[r*VGA_WIDTH+c];
    for (uint32_t c=0; c<VGA_WIDTH; ++c)
        vga[(VGA_HEIGHT-1u)*VGA_WIDTH+c] = ((uint16_t)color<<8)|' ';
    row = VGA_HEIGHT-1u;
}
void console_clear(void) {
    for (uint32_t i=0; i<VGA_WIDTH*VGA_HEIGHT; ++i)
        vga[i] = ((uint16_t)color<<8)|' ';
    row=0; col=0;
}
void console_init(void) { serial_init(); console_clear(); }
void console_putc(char c) {
    serial_write(c);
    if (c == '\r') { col=0; return; }
    if (c == '\n') { col=0; ++row; scroll_if_needed(); return; }
    if (c == '\b') {
        if (col) --col;
        else if (row) { --row; col=VGA_WIDTH-1u; }
        vga[row*VGA_WIDTH+col] = ((uint16_t)color<<8)|' ';
        return;
    }
    if ((uint8_t)c < 0x20u) return;
    vga[row*VGA_WIDTH+col] = ((uint16_t)color<<8)|(uint8_t)c;
    ++col;
    if (col>=VGA_WIDTH) { col=0; ++row; scroll_if_needed(); }
}
void console_write(const char *s) { while (*s) console_putc(*s++); }
void console_hex(uint32_t value) {
    const char *digits="0123456789ABCDEF";
    console_write("0x");
    for (int i=7; i>=0; --i) console_putc(digits[(value>>(i*4))&15u]);
}
void console_uint(uint32_t value) {
    char buf[10];
    uint32_t n=0;
    do { buf[n++]=(char)('0'+value%10u); value/=10u; } while (value);
    while (n) console_putc(buf[--n]);
}
int console_serial_read(void) {
    if (serial_available && (inb(COM1+5)&1)) return (int)inb(COM1);
    return -1;
}
