#ifndef _FB_H
#define _FB_H

#include "boot_info.h"

typedef struct rgb_color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_color;

typedef struct hsv_color
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} hsv_color;

void put_pixel(framebuffer_info_t *framebuffer_info, uint32_t x, uint32_t y, uint32_t c);
rgb_color hsv_to_rgb(hsv_color hsv);
hsv_color rgb_to_hsv(rgb_color rgb);
uint32_t rgb_to_color(rgb_color rgb);
void julia(framebuffer_info_t *framebuffer_info);
void clear(framebuffer_info_t *framebuffer_info);

#endif // _FB_H
