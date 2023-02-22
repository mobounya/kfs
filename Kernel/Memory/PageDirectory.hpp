#pragma once

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/Memory/PageTable.hpp>

#if !defined NULL
    #define NULL 0x00
#endif

// In 32-bit paging mode all paging structres are 1024 entries, 4 bytes for each entry.
#define N_PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_DIRECTORY_ENTRY_SIZE 4
#define PAGE_DIRECTORY_SIZE (N_PAGE_DIRECTORY_ENTRIES * PAGE_DIRECTORY_ENTRY_SIZE)

namespace Memory
{
    struct PageDirectory
    {
        const void                  *page_directory_ptr;
        PageDirectoryEntry          *page_directory[N_PAGE_DIRECTORY_ENTRIES];
        /*
            Use this to keep information about the page table referenced in [page_directory].
        */
        PageTableInfo               page_table_info[N_PAGE_DIRECTORY_ENTRIES];
        const PagingStructureEntry  *add_new_entry(PagingStructureEntry *entry, uint16_t index);
    };
}
