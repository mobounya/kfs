#include <cstring.h>
#include <Kernel/Memory/QuickDirtyMalloc.hpp>

char *strndup(const char *s, size_t n)
{
    size_t i = 0;
    char *tmp = (char *)quick_dirty_kmalloc(n + 1);
    for (; s[i] != '\0' && i < n; i++)
        tmp[i] = s[i];
    tmp[i] = '\0';
    return tmp;
}
