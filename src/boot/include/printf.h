#ifndef _PRINTF_H
#define _PRINTF_H

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

void kprintf_init(void (*putf)(wchar_t));
void kprintf(wchar_t* fmt, ...);
void ksprintf(wchar_t* s, wchar_t* fmt, ...);

#endif // _PRINTF_H
