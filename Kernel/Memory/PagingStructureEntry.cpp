#include <Kernel/Memory/PagingStructureEntry.hpp>

namespace Memory
{
    PagingStructureEntry *PagingStructureEntry::set_present(void)
    {
        this->present = 1;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_read_write(void)
    {
        this->read_write = 1;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_u_s(void)
    {
        this->u_s = 1;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_pwt(void)
    {
        this->pwt = 1;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_cache_disbled(void)
    {
        this->cache_disbled = 1;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_page_size(void)
    {
        this->ps = 1;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_pat(void)
    {
        this->pat = 1;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_global(void)
    {
        this->global = 1;
        return this;
    }

    PagingStructureEntry *PagingStructureEntry::set_physical_address(uint32_t physical_address)
    {
        // Address should be 4K page aligned.
        this->page_table_address = physical_address >> 12;
        return this;
    }
}
