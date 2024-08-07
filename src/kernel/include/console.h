#pragma once

#include "boot_info.h"

void
console_init(framebuffer_info_t *fb_info);

void
console_clear(void);

void
console_put_char(char c);
