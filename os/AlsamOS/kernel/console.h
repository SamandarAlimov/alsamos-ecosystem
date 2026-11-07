#ifndef ALSAMOS_CONSOLE_H
#define ALSAMOS_CONSOLE_H
#include <stdint.h>
void console_init(void);
void console_putc(char c);
void console_write(const char *s);
void console_hex(uint32_t value);
void console_uint(uint32_t value);
void console_clear(void);
int console_serial_read(void);
#endif
