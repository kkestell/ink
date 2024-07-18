#include "kprintf.h"
#include "console.h"
#include "fb.h"
#include "font.h"
#include "kalloc.h"

void draw_char(char ch, uint32_t x, uint32_t y, uint32_t color);
void scroll_screen(void);

static framebuffer_info_t *fb_info;
static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;
static const uint32_t color = 0xFFFFFF;
static uint32_t *backbuffer = NULL;

void console_init(framebuffer_info_t *info)
{
    fb_info = info;
    backbuffer = (uint32_t*)kmalloc(info->width * info->height * sizeof(uint32_t));
    if (!backbuffer) {
        kprintf("Failed to allocate backbuffer\n");
        while(1);  // Hang if we can't allocate memory
    }
    console_clear();
}

void console_clear(void)
{
    clear(fb_info);
    if (backbuffer) {
        for (uint32_t i = 0; i < fb_info->width * fb_info->height; i++) {
            backbuffer[i] = 0;
        }
    }
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
        scroll_screen();
        cursor_y = fb_info->height - FONT_HEIGHT;
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
                if (backbuffer) {
                    backbuffer[(y + row) * fb_info->width + (x + col)] = color;
                }
            }
        }
    }
}

void scroll_screen(void)
{
    if (!backbuffer) return;

    // Calculate the number of lines to scroll
    uint32_t lines_to_scroll = FONT_HEIGHT;

    // Move the screen content up by one line
    uint32_t pixels_to_move = fb_info->width * (fb_info->height - lines_to_scroll);
    uint32_t bytes_to_move = pixels_to_move * sizeof(uint32_t);
    
    // Move the backbuffer content
    memcpy(backbuffer, backbuffer + (fb_info->width * lines_to_scroll), bytes_to_move);

    // Clear the last line in the backbuffer
    memset(backbuffer + pixels_to_move, 0, fb_info->width * lines_to_scroll * sizeof(uint32_t));

    // Copy the backbuffer to the framebuffer
    memcpy((void*)fb_info->base_address, backbuffer, fb_info->width * fb_info->height * sizeof(uint32_t));
}