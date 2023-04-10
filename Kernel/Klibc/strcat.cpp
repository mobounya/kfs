#include <cstring.h>

char *strcat(char *dest, const char *src)
{
    size_t len = strlen(dest);
    size_t i = 0;
    for (; src[i] != 0; i++)
        dest[len + i] = src[i];
    dest[len + i] = '\0';
    return dest;
}
