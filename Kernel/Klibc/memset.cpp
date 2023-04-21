#include <cstring.h>

void *memset(void *s, int c, size_t n)
{
    char *temp_c = (char *)s;

    for (size_t i = 0; i < n; i++)
        temp_c[i] = c;

    return s;
}
