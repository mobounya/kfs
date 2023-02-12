#pragma once

#include <stdint.h>

namespace Memory
{
    class MemoryRegion
    {
        private:
            uint64_t    base_addr;
            uint64_t    length;
            uint8_t     type;
        public:
            MemoryRegion(uint64_t base_addr, uint64_t length, uint8_t type);
            uint64_t get_base_addr(void);
            uint64_t get_length(void);
            uint8_t  get_type(void);
    };
}