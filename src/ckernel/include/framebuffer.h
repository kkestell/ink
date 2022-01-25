#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include "boot_info.h"

typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;

void put_pixel(KernelBootInfo* boot_info, uint32_t x, uint32_t y, uint32_t c);
RgbColor hsv_to_rgb(HsvColor hsv);
HsvColor rgb_to_hsv(RgbColor rgb);
uint32_t rgb_to_color(RgbColor rgb);
void julia(KernelBootInfo* boot_info);
void clear(KernelBootInfo* boot_info);

#endif // _FRAMEBUFFER_H
