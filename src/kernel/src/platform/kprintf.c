#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include "types.h"

extern void uart_put_char(char c);
extern void uart_puts(char *s);

typedef void (*putc)(char);

static void
u64toa(u64 num, u8 base, char *bf)
{
    u64 d = 1;
    while (num / d >= base)
    {
        d *= base;
    }
    while (d != 0)
    {
        u64 dgt = num / d;
        num %= d;
        d /= base;
        *bf++ = dgt + (dgt < 10 ? '0' : 'a' - 10);
    }
    *bf = 0;
}

static void
u32toa(u32 num, u8 base, char *bf)
{
    u32 d = 1;

    while (num / d >= base)
    {
        d *= base;
    }

    while (d != 0)
    {
        u32 dgt = num / d;
        num %= d;
        d /= base;
        *bf++ = dgt + (dgt < 10 ? '0' : 'a' - 10);
    }
    *bf = 0;
}

static void
puts(char *bf)
{
    char c;
    while ((c = *bf++))
        uart_put_char(c);
}

static void
format(putc putf, char *fmt, va_list va)
{
    char bf[512];
    char ch;

    while ((ch = *(fmt++)))
    {
        if (ch != '%')
            putf(ch);
        else
        {
            ch = *(fmt++);

            switch (ch)
            {
                // 64-bit pointer, hex
                case 'p':
                    u64toa(va_arg(va, u64), 16, bf);
                    puts(bf);
                    break;
                // 64-bit integer
                case 'U':
                    u64toa(va_arg(va, u64), 10, bf);
                    puts(bf);
                    break;
                // 32-bit integer
                case 'u':
                    u32toa(va_arg(va, u32), 10, bf);
                    puts(bf);
                    break;
                default:
                    break;
            }
        }
    }
abort:;
}

void
kprintf(char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    format(uart_put_char, fmt, va);
    va_end(va);
}
