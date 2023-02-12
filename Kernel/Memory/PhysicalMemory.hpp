#pragma once

#include <User/c++/queue.hpp>
#include <Kernel/Memory/MemoryRegion.hpp>

namespace Memory
{
    class PhysicalMemory
    {
        private:
            std::queue<MemoryRegion> memory_regions;
        public:
            MemoryRegion  get_memory_region(void);
            void          add_memory_region(MemoryRegion& region);
    };
}
