#include <cstring.h>

size_t strlen(const char *str)
{
    size_t i = 0;
    for (; str[i] != '\0'; i++);
    return i;
}
