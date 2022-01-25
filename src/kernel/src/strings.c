#include <strings.h>

void bzero(void *p, size_t n)
{
  char *c = p;

  for (size_t i = 0; i < n; i++)
    c[i] = '\0';
}