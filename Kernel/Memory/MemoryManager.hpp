#pragma once

#include <stdint.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>

# define N_PAGING_STRUCTURE_ENTRIES 1024
# define PAGING_STRUCTURE_ENTRY_SIZE 4
# define PAGING_STRUCTURE_SIZE (N_PAGING_STRUCTURE_ENTRIES * PAGING_STRUCTURE_ENTRY_SIZE)

namespace Memory
{
    class MemoryManager
    {
        public:
            MemoryManager();

        public:
            void insert_page_directory_entry(page_directory_entry &entry);
            void enable_paging(void);
            void load_page_directory(void);

        private:
            paging_structure_entry page_directory[N_PAGING_STRUCTURE_ENTRIES];
            uint16_t index;
    };
};
