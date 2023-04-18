#pragma once

#include <stdint.h>
#include <stddef.h>

class Debug
{
    public:
        static void dump_registers(void);
        static void dump_stack(void);
};
