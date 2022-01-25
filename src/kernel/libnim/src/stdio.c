#include <stdio.h>

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