#pragma once

#include "boot_info.h";

void console_init(kernel_framebuffer_info_t *fb);
void console_puts(const char *str);
