#include <Kernel/Memory/QuickDirtyMalloc.hpp>
#include <cstring.h>

kmemory_area    small;
kmemory_area    medium;
kmemory_area    large;

extern "C"
{
    void    *kernel_memory_ptr;
}

/*
    This quick and dirty simple implementation of kmalloc
    is supposed to be used in the kernel before we initialize
    virtual memory and the memory interface.
*/

static int8_t find_unset_bit(uint64_t n)
{
    int8_t pos = 0;

    while (pos < 64)
    {
        if ((n & (1 << pos)) == 0)
            return pos;
        pos++;
    }
    return (-1);
}

static void    init_kmemory(void)
{
    small.bitmap = 0;
    small.ptr = kernel_memory_ptr;

    medium.bitmap = 0;
    medium.ptr = ((uint8_t *)kernel_memory_ptr) + (SMALL_AREA_SIZE * 100);

    large.bitmap = 0;
    large.ptr = ((uint8_t *)kernel_memory_ptr) + (SMALL_AREA_SIZE * 100) + (MEDIUM_AREA_SIZE * 100);
}

/*
    This will get you the actual allocated memory when you pass n to quick_dirty_kmalloc
*/

size_t  get_actual_allocated_size(size_t n)
{
    if (n < SMALL_AREA_SIZE)
        return SMALL_AREA_SIZE - 1;
    else if (n < MEDIUM_AREA_SIZE)
        return MEDIUM_AREA_SIZE - 1;
    else if (n < LARGE_AREA_SIZE)
        return LARGE_AREA_SIZE - 1;
    else
        return 0;
}

void    *quick_dirty_kmalloc(size_t n)
{
    static bool initialized = false;
    void        *ptr;
 
    if (initialized == false)
    {
        init_kmemory();
        initialized = true;
    }

    if (n < SMALL_AREA_SIZE)
    {
        int8_t unset_bit = find_unset_bit(small.bitmap);
        ptr = ((uint8_t *)small.ptr) + (unset_bit * SMALL_AREA_SIZE);
        *((uint8_t *)ptr) = SMALL_AREA_FLAG;
        ptr = ((uint8_t *)ptr) + 1;
        memset(ptr, 0, SMALL_AREA_SIZE - 1);
        small.bitmap |= 1 << unset_bit;
        return ptr;
    }
    else if (n < MEDIUM_AREA_SIZE)
    {
        int8_t unset_bit = find_unset_bit(medium.bitmap);
        ptr = ((uint8_t *)medium.ptr) + (unset_bit * MEDIUM_AREA_SIZE);
        *((uint8_t*)ptr) = MEDIUM_AREA_FLAG;
        ptr = ((uint8_t*)ptr) + 1;
        memset(ptr, 0, SMALL_AREA_SIZE - 1);
        medium.bitmap |= 1 << unset_bit;
        return ptr;
    }
    else if (n < LARGE_AREA_SIZE)
    {
        int8_t unset_bit = find_unset_bit(large.bitmap);
        ptr = ((uint8_t *)large.ptr) + (unset_bit * LARGE_AREA_SIZE);
        *((uint8_t*)ptr) = LARGE_AREA_FLAG;
        ptr = ((uint8_t*)ptr) + 1;
        memset(ptr, 0, SMALL_AREA_SIZE - 1);
        large.bitmap |= 1 << unset_bit;
        return ptr;
    }
    return NULL;
}

void    quick_dirty_kfree(const void *ptr)
{
    ptr = ((uint8_t *)ptr) - 1;
    if (*((uint8_t *)ptr) == SMALL_AREA_FLAG)
    {
        uint32_t offset = (uint32_t)ptr - (uint32_t)(small.ptr);
        small.bitmap &= ~(1 << (offset / SMALL_AREA_SIZE));
    }
    else if (*((uint8_t *)ptr) == MEDIUM_AREA_FLAG)
    {
        uint32_t offset = (uint32_t)ptr - (uint32_t)(medium.ptr);
        medium.bitmap &= ~(1 << (offset / MEDIUM_AREA_SIZE));
    }
    else if (*((uint8_t *)ptr) == LARGE_AREA_FLAG)
    {
        uint32_t offset = (uint32_t)ptr - (uint32_t)(large.ptr);
        large.bitmap &= ~(1 << (offset / LARGE_AREA_SIZE));
    }
}