#include <format.hpp>
#include <Kernel/Memory/QuickDirtyMalloc.hpp>

#include <cstring.h>

char    *to_hex(int n)
{
    char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    size_t size = 18;
    char *buffer = (char *)quick_dirty_kmalloc(size);
    char *hex_string;
    buffer[size--] = '\0';
    
    size = size - 1;
    while (n >= 0)
    {
        buffer[size] = hex[n % 16];
        n = n / 16;
        size--;
        if (n == 0)
            break;
    }

    hex_string = strdup(buffer + size + 1);
    quick_dirty_kfree(buffer);
    return hex_string;
}
