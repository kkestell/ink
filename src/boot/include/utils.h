#ifndef __BOOT_UTILS_H
#define __BOOT_UTILS_H

#include <stdarg.h>
#include <stdint.h>

void kprintf_init(void *putp, void (*putf)(void*, uint16_t));
void kprintf(uint16_t *fmt, ...);
void ksprintf(uint16_t *s, uint16_t *fmt, ...);

#endif // __BOOT_UTILS_H
