#pragma once

#include <Kernel/Memory/MemoryRegion.hpp>

#define PAGE_SIZE 4096

namespace Memory
{
    class MemoryPage : public MemoryRegion
    {
        public:
            MemoryPage(uint64_t base_addr);
    };
}
