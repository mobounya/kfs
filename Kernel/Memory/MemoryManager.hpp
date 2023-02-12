#pragma once

#include <stdint.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <User/c++/queue.hpp>
#include <Kernel/Memory/MemoryRegion.hpp>
#include <Kernel/Memory/PhysicalMemory.hpp>

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
            void add_physical_memory_region(MemoryRegion &region);

        private:
            PhysicalMemory physical_memory;
            paging_structure_entry page_directory[N_PAGING_STRUCTURE_ENTRIES];
            uint16_t index;
    };
};
