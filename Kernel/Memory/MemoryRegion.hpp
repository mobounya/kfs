#pragma once

#include <stdint.h>

namespace Memory
{
    class MemoryRegion
    {
        protected:
            uint32_t    base_addr;
            uint32_t    length;
            uint8_t     type;

        public:
            MemoryRegion();
            MemoryRegion(uint32_t base_addr, uint32_t length, uint8_t type);

        public:
            void        set_base_addr(uint32_t base_addr);
            uint32_t    get_base_addr(void) const;
            void        set_length(uint32_t length);
            uint32_t    get_length(void) const;
            void        set_type(uint8_t type);
            uint8_t     get_type(void) const;
    };
}
