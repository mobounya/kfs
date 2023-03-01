#include <Kernel/Memory/PagingStructureEntry.hpp>

namespace Memory
{
    PagingStructureEntry *PagingStructureEntry::set_present(bool value)
    {
        this->present = value;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_read_write(bool value)
    {
        this->read_write = value;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_u_s(bool value)
    {
        this->u_s = value;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_pwt(bool value)
    {
        this->pwt = value;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_cache_disbled(bool value)
    {
        this->cache_disbled = value;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_page_size(bool value)
    {
        this->pat = value;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_pat(bool value)
    {
        this->pat = value;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_global(bool value)
    {
        this->global = value;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_physical_address(uint32_t physical_address)
    {
        // Address should be 4K page aligned.
        this->physical_address = physical_address >> 12;
        return this;
    }
}
