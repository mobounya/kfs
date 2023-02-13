#pragma once

#include <Kernel/Memory/PagingStructureEntry.hpp>

// In 32-bit paging mode all paging structres are 1024 entries, 4 bytes for each entry.
#define N_PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_DIRECTORY_ENTRY_SIZE 4
#define PAGE_DIRECTORY_SIZE (N_PAGE_DIRECTORY_ENTRIES * PAGE_DIRECTORY_ENTRY_SIZE)

namespace Memory
{
    struct PageDirectory
    {
        PageDirectoryEntry page_directory[N_PAGE_DIRECTORY_ENTRIES];
        void add_new_entry(const PageDirectoryEntry &entry, uint16_t index);
    };
}
