#pragma once

#include <Kernel/Memory/PagingStructureEntry.hpp>

#define N_PAGE_DIRECTORY_ENTRIES 1024

namespace Memory
{
    struct PageDirectory
    {
        page_directory_entry page_directory[N_PAGE_DIRECTORY_ENTRIES];
        void add_new_entry(const PagingStructureEntry &entry, uint16_t index);
    };
}
