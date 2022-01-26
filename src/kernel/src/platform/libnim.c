#include <stddef.h>
#include <stdio.h>

#include "kalloc.h"

// signal.h

void (*signal(int sig, void (*func)(int)))(int)
{
}

int raise(int sig)
{
    return 0;
}

// stdlib.h

void *malloc(size_t size)
{
    kmalloc(size);
}

void free(void *block)
{
    kfree(block);
}

void *calloc(size_t number, size_t size)
{
    kcalloc(number, size);
}

void *realloc(void *ptr, size_t size)
{
    krealloc(ptr, size);
}

// stdio.h

size_t fwrite(const void *restrict src, size_t size, size_t nmemb, FILE *restrict f)
{
    return size;
}

int fflush(FILE *f)
{
    return 0;
}

void exit(int code)
{
}

FILE __stderr_FILE;
FILE *stderr = &__stderr_FILE;

// string.h

void *memset(void *b, int c, size_t len)
{
    int i;
    unsigned char *p = b;
    i = 0;

    while(len > 0)
    {
        *p = c;
        p++;
        len--;
    }
  
    return(b);
}

void *memcpy(void *s1, const void *s2, size_t n)
{
    char *p1 = (char *)s1;
    const char *p2 = (const char *)s2;

    while (n-- > 0)
    {
        *p1 = *p2;
        p1++;
        p2++;
    }
    
    return s1;
}

size_t strlen(const char *str)
{
    size_t n;
    for (n = 0; *str != '\0'; str++, n++);
    return n;
}

// strings.h

void bzero(void *p, size_t n)
{
  char *c = p;

  for (size_t i = 0; i < n; i++)
    c[i] = '\0';
}

