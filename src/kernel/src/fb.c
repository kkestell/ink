#include <stddef.h>
#include "kalloc.h"
#include "fb.h"

void put_pixel(framebuffer_info_t *framebuffer_info, uint32_t x, uint32_t y, uint32_t c)
{
    *((uint32_t*)(framebuffer_info->base_address + 4 * framebuffer_info->pixels_per_scan_line * y + 4 * x)) = c;
}

rgb_color hsv_to_rgb(hsv_color hsv)
{
    rgb_color rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;

        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

hsv_color rgb_to_hsv(rgb_color rgb)
{
    hsv_color hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * (long)(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

uint32_t rgb_to_color(rgb_color rgb)
{
    uint32_t color = rgb.r;
    color = (color << 8) + rgb.g;
    color = (color << 8) + rgb.b;

    return color;
}

void julia(framebuffer_info_t *framebuffer_info)
{
    int32_t w = framebuffer_info->width;
    int32_t h = framebuffer_info->height;
    double cRe, cIm;
    double newRe, newIm, oldRe, oldIm;
    double zoom = 1, moveX = 0, moveY = 0;
    int32_t maxIterations = 300;

    cRe = -0.7;
    cIm = 0.27015;

    for (int32_t y = 0; y < h; y++)
    {
        for (int32_t x = 0; x < w; x++)
        {
            newRe = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
            newIm = (y - h / 2) / (0.5 * zoom * h) + moveY;
            int32_t i;
            for (i = 0; i < maxIterations; i++)
            {
                oldRe = newRe;
                oldIm = newIm;

                newRe = oldRe * oldRe - oldIm * oldIm + cRe;
                newIm = 2 * oldRe * oldIm + cIm;

                if ((newRe * newRe + newIm * newIm) > 4)
                    break;
            }

            hsv_color c;
            c.h = i % 256;
            c.s = 255;
            c.v = 255 * (i < maxIterations);

            put_pixel(framebuffer_info, x, y, rgb_to_color(hsv_to_rgb(c)));
        }
    }
}

void clear(framebuffer_info_t *framebuffer_info)
{
    // Ensure the color is in the correct format expected by the framebuffer
    uint32_t c = 0x000000; // Assuming a black color and the framebuffer uses 32-bit color depth

    // Calculate the total size of the framebuffer in bytes
    size_t bufferSize = framebuffer_info->pixels_per_scan_line * framebuffer_info->height * sizeof(uint32_t);

    // Use memset to fill the framebuffer with the color
    memset((void*)framebuffer_info->base_address, c, bufferSize);
}
