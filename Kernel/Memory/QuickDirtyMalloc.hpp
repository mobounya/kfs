#pragma once

#include <stdint.h>
#include <stddef.h>

#define SMALL_AREA_FLAG 0x1
#define MEDIUM_AREA_FLAG 0x2
#define LARGE_AREA_FLAG 0x3

#define SMALL_AREA_SIZE 51
#define MEDIUM_AREA_SIZE 251
#define LARGE_AREA_SIZE 501

struct kmemory_area
{
    uint64_t    bitmap;
    void        *ptr;
};

size_t  get_actual_allocated_size(size_t n);
void    quick_dirty_kfree(const void *ptr);
void    *quick_dirty_kmalloc(size_t n);
