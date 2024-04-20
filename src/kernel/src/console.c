#include "kprintf.h"
#include "console.h"
#include "fb.h"
#include "font.h"

void draw_char(char ch, uint32_t x, uint32_t y, uint32_t color);

static framebuffer_info_t *fb_info;
static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;
static const uint32_t color = 0xFFFFFF;

void console_init(framebuffer_info_t *info)
{
    fb_info = info;
    console_clear();
}

void console_clear(void)
{
    clear(fb_info);
    cursor_x = 0;
    cursor_y = 0;
}

void console_putc(char c)
{
    switch (c)
    {
        case '\n':
            cursor_x = 0;
            cursor_y += FONT_HEIGHT;
            break;
        case '\r':
            cursor_x = 0;
            break;
        default:
            draw_char(c, cursor_x, cursor_y, color);
            cursor_x += FONT_WIDTH;
            break;
    }

    if (cursor_x + FONT_WIDTH > fb_info->width)
    {
        cursor_x = 0;
        cursor_y += FONT_HEIGHT;
    }

    if (cursor_y + FONT_HEIGHT > fb_info->height)
    {
        // TODO: Implement scrolling
        cursor_y = 0;
    }
}

void console_puts(const char *s)
{
    while (*s)
    {
        console_putc(*s++);
    }
}

void draw_char(char ch, uint32_t x, uint32_t y, uint32_t color)
{
    for (int row = 0; row < FONT_HEIGHT; row++)
    {
        for (int col = 0; col < FONT_WIDTH; col++)
        {
            int bit_idx = col % 8;
            int byte_idx = col / 8;
            if (font[(int)ch][row][byte_idx] & (1 << (7 - bit_idx)))
            {
                put_pixel(fb_info, x + col, y + row, color);
            }
        }
    }
}
