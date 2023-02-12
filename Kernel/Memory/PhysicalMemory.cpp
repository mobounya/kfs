#include <Kernel/Memory/PhysicalMemory.hpp>

namespace Memory
{
    MemoryRegion PhysicalMemory::get_memory_region(void)
    {
        MemoryRegion region = memory_regions.front();
        memory_regions.pop();
        return region;
    }

    void    PhysicalMemory::add_memory_region(MemoryRegion& region)
    {
        memory_regions.push(region);
    }
}
