#pragma once

#include "boot_info.h"

void console_init(framebuffer_info_t *fb_info);
void console_clear(void);
void console_putc(char c);
void console_puts(const char *s);
