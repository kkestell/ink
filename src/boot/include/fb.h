#ifndef __BOOT_FB_H
#define __BOOT_FB_H

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

void fb_init();

#endif
