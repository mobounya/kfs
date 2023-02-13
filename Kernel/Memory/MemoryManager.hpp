#pragma once

#include <stdint.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <User/c++/queue.hpp>
#include <Kernel/Memory/MemoryRegion.hpp>
#include <Kernel/Memory/PhysicalMemory.hpp>
#include <Kernel/Memory/PageDirectory.hpp>
#include <Kernel/Memory/PageTable.hpp>

namespace Memory
{
    class MemoryManager
    {
        public:
            MemoryManager(PageDirectory *page_directory);

        public:
            void insert_page_directory_entry(const PagingStructureEntry &entry);
            void insert_page_table_entry(const PageTableEntry &entry);
            void enable_paging(void);
            void load_page_directory(void);
            void add_physical_memory_region(const MemoryRegion &region);

        private:
            /* 
                These are the physical memory regions detected by GRUB, we only store usable [MULTIBOOT_MEMORY_AVAILABLE] for now.
                Notes from https://wiki.osdev.org/Detecting_Memory_(x86) :
                    - (in rare/dodgy cases) may return overlapping areas
                    - These are not necessarily 4K page alligned.
            */
            PhysicalMemory  physical_memory;
            PageDirectory   *page_directory;
            uint16_t        page_directory_size;
            PageTable       *page_table;
            uint16_t        page_table_size;
    };
};
