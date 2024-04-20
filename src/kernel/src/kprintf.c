#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "uart.h"
#include "console.h"

typedef void (*putc)(char);

static void u64toa(uint64_t num, uint8_t base, char *bf)
{
    uint64_t d = 1;
    while (num / d >= base)
    {
        d *= base;
    }
    while (d != 0)
    {
        uint64_t dgt = num / d;
        num %= d;
        d /= base;
        *bf++ = dgt + (dgt < 10 ? '0' : 'a' - 10);
    }
    *bf = 0;
}

static void u32toa(uint32_t num, uint8_t base, char *bf)
{
    uint32_t d = 1;

    while (num / d >= base)
    {
        d *= base;
    }

    while (d != 0)
    {
        uint32_t dgt = num / d;
        num %= d;
        d /= base;
        *bf++ = dgt + (dgt < 10 ? '0' : 'a' - 10);
    }
    *bf = 0;
}

static void puts(char *bf)
{
    char c;
    while ((c = *bf++))
        console_putc(c);
}

static void format(putc putf, char *fmt, va_list va)
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
                    u64toa(va_arg(va, uint64_t), 16, bf);
                    puts("0x");
                    puts(bf);
                    break;
                // 64-bit integer
                case 'U':
                    u64toa(va_arg(va, uint64_t), 10, bf);
                    puts(bf);
                    break;
                // 32-bit integer
                case 'u':
                    u32toa(va_arg(va, uint32_t), 10, bf);
                    puts(bf);
                    break;
                default:
                    break;
            }
        }
    }
}

void kprintf(char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    format(console_putc, fmt, va);
    va_end(va);
}
