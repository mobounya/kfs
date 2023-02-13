#pragma once

#include <stdint.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <User/c++/queue.hpp>
#include <Kernel/Memory/MemoryRegion.hpp>
#include <Kernel/Memory/PhysicalMemory.hpp>
#include <Kernel/Memory/PageDirectory.hpp>

namespace Memory
{
    class MemoryManager
    {
        public:
            MemoryManager(PageDirectory *page_directory);

        public:
            void insert_page_directory_entry(const PagingStructureEntry &entry);
            void enable_paging(void);
            void load_page_directory(void);
            void add_physical_memory_region(const MemoryRegion &region);

        private:
            PhysicalMemory  physical_memory;
            PageDirectory   *page_directory;
            uint16_t        page_directory_size;
    };
};
