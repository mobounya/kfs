#include <cstring.h>

void *memcpy(void *dst, const void *src, size_t n)
{
    uint8_t *c_dst = (uint8_t *)dst;
    uint8_t *c_src = (uint8_t *)src;

    for (size_t i = 0; i < n; i++)
        c_dst[i] = c_src[i];
    return dst;
}
