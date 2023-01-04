#include "not_libc.hpp"

size_t ft_strlen(const char *str)
{
    size_t i = 0;
    for (; str[i] != '\0'; i++);
    return i;
}