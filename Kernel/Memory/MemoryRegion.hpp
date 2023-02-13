#pragma once

#include <stdint.h>

namespace Memory
{
    class MemoryRegion
    {
        protected:
            uint64_t    base_addr;
            uint64_t    length;
            uint8_t     type;
        public:
            MemoryRegion();
            MemoryRegion(uint64_t base_addr, uint64_t length, uint8_t type);
            void set_base_addr(uint64_t base_addr);
            uint64_t get_base_addr(void) const;
            void set_length(uint64_t length);
            uint64_t get_length(void) const;
            void  set_type(uint8_t type);
            uint8_t  get_type(void) const;
    };
}
