#include <Kernel/Memory/MemoryRegion.hpp>

namespace Memory
{
    MemoryRegion::MemoryRegion()
    {

    }

    MemoryRegion::MemoryRegion(uint32_t base_addr, uint32_t length, uint8_t type) : base_addr(base_addr), length(length),
                                type(type)
    {

    }

    void MemoryRegion::set_base_addr(uint32_t base_addr)
    {
        this->base_addr = base_addr;
    }

    uint32_t MemoryRegion::get_base_addr(void) const
    {
        return this->base_addr;
    }

    void MemoryRegion::set_length(uint32_t length)
    {
        this->length = length;
    }

    uint32_t MemoryRegion::get_length(void) const
    {
        return this->length;
    }

    void  MemoryRegion::set_type(uint8_t type)
    {
        this->type = type;
    }

    uint8_t  MemoryRegion::get_type(void) const
    {
        return this->type;
    }
}
