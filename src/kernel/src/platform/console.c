#include <stddef.h>
#include <string.h>
#include "console.h"

// ./sfnconv -U ProggyCleanSZ.pcf font_proggy.sfn

//extern unsigned char _binary_data_font_proggy_sfn_start;
extern unsigned char _binary_data_font_terminus_sfn_start;

#define SSFN_CONSOLEBITMAP_TRUECOLOR
#define SSFN_CONSOLEBITMAP_CONTROL

#include "ssfn.h"

void console_init(kernel_framebuffer_info_t *fb)
{
    ssfn_src     = (ssfn_font_t *)&_binary_data_font_terminus_sfn_start;
    ssfn_dst.ptr = (unsigned char *)fb->framebuffer_base_address;
    ssfn_dst.w   = fb->framebuffer_width;
    ssfn_dst.h   = fb->framebuffer_height;
    ssfn_dst.p   = fb->framebuffer_pixels_per_scan_line * 4;
    ssfn_dst.x   = ssfn_dst.y = 0;
    ssfn_dst.fg  = 0xFFFFFF;
}

void console_puts(const char *str)
{
    for(size_t i = 0; i < strlen(str); i++) 
        ssfn_putc(str[i]);
}
