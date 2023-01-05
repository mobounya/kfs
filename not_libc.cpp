#include "not_libc.hpp"

size_t strlen(const char *str)
{
    size_t i = 0;
    for (; str[i] != '\0'; i++);
    return i;
}

int strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    while (s1[i] == s2[i])
        ++i;
    return s1[i] - s2[i];
}
