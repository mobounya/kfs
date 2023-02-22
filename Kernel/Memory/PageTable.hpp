#pragma once

#include <Kernel/Memory/PagingStructureEntry.hpp>

#if !defined NULL
    #define NULL 0x00
#endif

// In 32-bit paging mode all paging structres are 1024 entries, 4 bytes for each entry.
#define N_PAGE_TABLE_ENTRIES 1024
#define PAGE_TABLE_ENTRY_SIZE 4
#define PAGE_TABLE_SIZE (N_PAGE_TABLE_ENTRIES * PAGE_TABLE_ENTRY_SIZE)

namespace Memory
{
    struct PageTable
    {
        PageTableEntry          page_table[N_PAGE_TABLE_ENTRIES];
        const PageTableEntry    &add_new_entry(const PageTableEntry &entry, uint16_t index);
    };
}
