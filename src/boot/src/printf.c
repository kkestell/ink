#include "printf.h"

typedef void (*putcf)(wchar_t);
static putcf stdout_putf;

static void uli2a(unsigned long int num, unsigned int base, int uc, wchar_t *bf)
{
    int n = 0;
    unsigned int d = 1;
    while (num / d >= base)
    {
        d *= base;
    }
    while (d != 0)
    {
        int dgt = num / d;
        num %= d;
        d /= base;
        if (n || dgt > 0 || d == 0)
        {
            *bf++ = dgt + (dgt < 10 ? '0' : (uc ? 'A' : 'a') - 10);
            ++n;
        }
    }
    *bf = 0;
}

static void li2a(long num, wchar_t* bf)
{
    if (num < 0)
    {
        num = -num;
        *bf++ = '-';
    }
    uli2a(num, 10, 0, bf);
}

static void ui2a(unsigned int num, unsigned int base, int uc, wchar_t *bf)
{
    int n = 0;
    unsigned int d = 1;
    while (num / d >= base)
    {
        d *= base;
    }
    while (d != 0)
    {
        int dgt = num / d;
        num %= d;
        d /= base;
        if (n || dgt > 0 || d == 0)
        {
            *bf++ = dgt + (dgt < 10 ? '0' : (uc ? 'A' : 'a') - 10);
            ++n;
        }
    }
    *bf = 0;
}

static void i2a(int num, wchar_t *bf)
{
    if (num < 0)
    {
        num = -num;
        *bf++ = '-';
    }
    ui2a(num, 10, 0, bf);
}

static int a2d(wchar_t ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    else if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    else
        return -1;
}

static wchar_t a2i(wchar_t ch, wchar_t **src, int base, int *nump)
{
    wchar_t *p = *src;
    int num = 0;
    int digit;
    while ((digit = a2d(ch)) >= 0)
    {
        if (digit > base)
            break;
        num = num * base + digit;
        ch = *p++;
    }
    *src = p;
    *nump = num;
    return ch;
}

static void putchw(putcf putf, int n, wchar_t z, wchar_t *bf)
{
    wchar_t fc = z ? '0' : ' ';
    wchar_t ch;
    wchar_t *p = bf;
    while (*p++ && n > 0)
        n--;
    while (n-- > 0)
        putf(fc);
    while ((ch = *bf++))
        putf(ch);
}

static void format(putcf putf, wchar_t *fmt, va_list va)
{
    wchar_t bf[12];

    wchar_t ch;

    while ((ch = *(fmt++)))
    {
        if (ch != '%')
            putf(ch);
        else
        {
            wchar_t lz = 0;
            wchar_t lng = 0;
            int w = 0;
            ch = *(fmt++);
            if (ch == '0')
            {
                ch = *(fmt++);
                lz = 1;
            }
            if (ch >= '0' && ch <= '9')
            {
                ch = a2i(ch, &fmt, 10, &w);
            }
            if (ch == 'l')
            {
                ch = *(fmt++);
                lng = 1;
            }
            switch (ch)
            {
            case 0:
                goto abort;
            case 'u':
            {
                if (lng)
                    uli2a(va_arg(va, unsigned long int), 10, 0, bf);
                else
                    ui2a(va_arg(va, unsigned int), 10, 0, bf);
                putchw(putf, w, lz, bf);
                break;
            }
            case 'd':
            {
                if (lng)
                    li2a(va_arg(va, unsigned long int), bf);
                else
                    i2a(va_arg(va, int), bf);
                putchw(putf, w, lz, bf);
                break;
            }
            case 'x':
            case 'X':
                if (lng)
                    uli2a(va_arg(va, unsigned long int), 16, (ch == 'X'), bf);
                else
                    ui2a(va_arg(va, unsigned int), 16, (ch == 'X'), bf);
                putchw(putf, w, lz, bf);
                break;
            case 'c':
                putf((wchar_t)(va_arg(va, int)));
                break;
            case 's':
                putchw(putf, w, 0, va_arg(va, wchar_t*));
                break;
            case '%':
                putf(ch);
            default:
                break;
            }
        }
    }
abort:;
}

void kprintf_init(void (*putf)(wchar_t))
{
    stdout_putf = putf;
}

void kprintf(wchar_t *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    format(stdout_putf, fmt, va);
    va_end(va);
}
