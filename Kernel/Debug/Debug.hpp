#pragma once

#include <stdint.h>
#include <stddef.h>

class Debug
{
    public:
        static void print_mem_line(void *ptr, size_t size);
        static void dump_registers(void);
        static void dump_stack(void);
        static void print_mem(void *ptr, size_t size);
};
