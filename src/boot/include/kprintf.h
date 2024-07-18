#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

void kprintf_init(void (*putf)(wchar_t));
void die(wchar_t *fmt, ...);
void kprintf(wchar_t *fmt, ...);
void ksprintf(wchar_t *s, wchar_t *fmt, ...);
