#pragma once

#include <stdint.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <User/c++/queue.hpp>
#include <Kernel/Memory/MemoryRegion.hpp>
#include <Kernel/Memory/PhysicalMemory.hpp>
#include <Kernel/Memory/PageDirectory.hpp>
#include <Kernel/Memory/PageTable.hpp>
#include <Kernel/Memory/MemoryPage.hpp>

namespace Memory
{
    class MemoryManager
    {
        public:
            MemoryManager();
            MemoryManager(PageDirectory *page_directory);

        public:
            void                insert_page_directory_entry(const PagingStructureEntry &entry);
            void                insert_page_table_entry(const PageTableEntry &entry);
            void                enable_paging(void);
            void                load_page_directory(void);
            void                declare_memory_region(const MemoryRegion &region);
            const MemoryPage    *allocate_memory_page(void);
            uint64_t            find_aligned_address(uint64_t address, uint64_t alignment) const;

        private:
            /* 
                These are the physical memory regions detected by GRUB, we only store usable [MULTIBOOT_MEMORY_AVAILABLE] for now.
                Notes from https://wiki.osdev.org/Detecting_Memory_(x86) :
                    - (in rare/dodgy cases) may return overlapping areas
                    - These are not necessarily 4K page aligned.
                FIXME: divide overlapping memory regions.
            */
            PhysicalMemory<MemoryRegion>    physical_memory;
            PhysicalMemory<MemoryPage>      allocated_memory_pages;
            PhysicalMemory<MemoryPage>      free_memory_pages;
            PageDirectory                   *page_directory;
            uint16_t                        page_directory_size;
            PageTable                       *page_table;
            uint16_t                        page_table_size;
    };
};
