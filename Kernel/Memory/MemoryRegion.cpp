#include <Kernel/Memory/MemoryRegion.hpp>

namespace Memory
{
    MemoryRegion::MemoryRegion(uint64_t base_addr, uint64_t length, uint8_t type) : base_addr(base_addr), length(length),
                                type(type)
    {

    }

    uint64_t MemoryRegion::get_base_addr(void)
    {
        return this->base_addr;
    }

    uint64_t MemoryRegion::get_length(void)
    {
        return this->length;
    }

    uint8_t  MemoryRegion::get_type(void)
    {
        return this->type;
    }
}
