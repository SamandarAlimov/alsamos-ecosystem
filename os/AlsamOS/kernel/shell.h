#ifndef ALSAMOS_SHELL_H
#define ALSAMOS_SHELL_H
#include <stdint.h>
void shell_init(uint32_t mem_upper_kib, int memory_available);
void shell_poll(void);
#endif
