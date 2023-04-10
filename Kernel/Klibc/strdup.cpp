#include <cstring.h>
#include <Kernel/Memory/QuickDirtyMalloc.hpp>

char *strdup(const char *str)
{
    size_t i = 0;
    size_t str_len = strlen(str);
    char *tmp = (char *)quick_dirty_kmalloc(str_len + 1);
    for (; i < str_len; i++)
        tmp[i] = str[i];
    tmp[i] = '\0';
    return tmp;
}
