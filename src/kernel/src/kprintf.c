#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "uart.h"
#include "console.h"

typedef void (*putc)(char);

static void u64toa(uint64_t num, uint8_t base, char *bf, int width)
{
    char temp[65];
    char *p = temp;
    uint64_t d = 1;
    int digit_count = 0;

    // Convert number to string in reverse order
    do
    {
        uint64_t dgt = num % base;
        *p++ = dgt + (dgt < 10 ? '0' : 'A' - 10);
        num /= base;
        digit_count++;
    }
    while (num > 0);

    // Pad with zeros if necessary
    while (digit_count < width)
    {
        *p++ = '0';
        digit_count++;
    }

    // Reverse the string
    while (p > temp)
    {
        *bf++ = *--p;
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
        *bf++ = dgt + (dgt < 10 ? '0' : 'A' - 10);
    }

    *bf = 0;
}

void put_char(char c)
{
    console_put_char(c);
    
    if (uart_is_available()) 
    {
        uart_put_char(c);
    }
}

static void puts(void (*putf)(char), char *bf)
{
    while (*bf)
    {
        putf(*bf++);
    }
}

static void format(void (*putf)(char), char *fmt, va_list va)
{
    char bf[512];
    char ch;

    while ((ch = *(fmt++)))
    {
        if (ch != '%')
        {
            putf(ch);
        }
        else
        {
            ch = *(fmt++);
            switch (ch)
            {
                case 'p':
                    u64toa((uint64_t)(intptr_t)va_arg(va, void*), 16, bf, 16);
                    puts(putf, "0x");
                    puts(putf, bf);
                    break;
                case 'U':
                case 'u':
                    u64toa(va_arg(va, uint64_t), 10, bf, 1);
                    puts(putf, bf);
                    break;
                case 'd':
                {
                    int num = va_arg(va, int);
                    if (num < 0)
                    {
                        putf('-');
                        num = -num;
                    }
                    u32toa((uint32_t)num, 10, bf);
                    puts(putf, bf);
                    break;
                }
                case 'x':
                    u32toa(va_arg(va, uint32_t), 16, bf);
                    puts(putf, "0x");
                    puts(putf, bf);
                    break;
                case 's':
                    puts(putf, va_arg(va, char *));
                    break;
                case 'c':
                {
                    char c = (char) va_arg(va, int);
                    putf(c);
                    break;
                }
                default:
                    putf('%');
                    putf(ch);
                    break;
            }
        }
    }
}

void kprintf(char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    format(put_char, fmt, va);
    va_end(va);
}

void kprintf_test(void)
{
    // Test string
    kprintf("Test string: %s\n", "Hello, World!");

    // Test character
    kprintf("Test character: %c\n", 'A');

    // Test decimal integers
    kprintf("Test positive decimal: %d\n", 12345);
    kprintf("Test negative decimal: %d\n", -6789);
    kprintf("Test zero decimal: %d\n", 0);

    // Test unsigned integers
    kprintf("Test unsigned (u): %u\n", 4294967295U);
    kprintf("Test unsigned (U): %U\n", 18446744073709551615ULL);

    // Test hexadecimal
    kprintf("Test hexadecimal: %x\n", 0xABCD);
    kprintf("Test hexadecimal with leading zeros: %x\n", 0x123);

    // Test pointer
    int test_var = 42;
    kprintf("Test pointer: %p\n", (void*)&test_var);

    // Test multiple arguments
    kprintf("Multiple args: %d %u %x %s\n", 10, 20U, 0x30, "forty");

    // Test edge cases
    kprintf("Max u32: %u\n", 0xFFFFFFFFU);
    kprintf("Max u64: %U\n", 0xFFFFFFFFFFFFFFFFULL);
    kprintf("Min int: %d\n", -2147483648);

    // Test format specifiers with no arguments
    kprintf("Percent sign: %%\n");

    // Test unrecognized format specifier
    kprintf("Unrecognized specifier: %z\n");

    // Test empty format string
    kprintf("");

    // Test long string
    kprintf("Long string: %s\n", "This is a very long string that tests the ability of kprintf to handle strings that might be longer than the internal buffer size. It's important to ensure that the function can handle such cases gracefully without buffer overflows.");

    // Test all format specifiers in one call
    kprintf("All specifiers: %c %s %d %u %U %x %p %%\n", 'X', "test", -100, 200U, 300ULL, 0xDEAD, (void*)0x1234);

    // Test consecutive format specifiers
    kprintf("Consecutive specifiers: %d%d %s%s\n", 1, 2, "three", "four");
}
