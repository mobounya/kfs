#include <cstring.h>

char *strcpy(char *dst, const char *src)
{
    for (size_t i = 0; src[i] != '\0'; i++)
        dst[i] = src[i];
    return dst;
}
