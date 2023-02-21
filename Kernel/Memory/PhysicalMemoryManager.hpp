#pragma once

#include <stdint.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <User/c++/queue.hpp>
#include <Kernel/Memory/MemoryRegion.hpp>
#include <Kernel/Memory/PhysicalMemory.hpp>
#include <Kernel/Memory/PageDirectory.hpp>
#include <Kernel/Memory/PageTable.hpp>
#include <Kernel/Memory/MemoryPage.hpp>
#include <Kernel/VGA/VGA.hpp>

namespace Memory
{
    class PhysicalMemoryManager
    {
        private:
            PhysicalMemoryManager();

        public:
            static PhysicalMemoryManager       &instantiate(void);
            void                                enable_paging(void);
            void                                declare_memory_region(const MemoryRegion &region);
            const MemoryPage                    *allocate_physical_memory_page(void);
            void                                free_physical_memory_page(const MemoryPage &page);
            static uint64_t                     find_aligned_address(uint64_t address, uint64_t alignment);

        public:
            void                        reserve_physical_memory(uint64_t physical_address_start, uint64_t physical_address_end);

            /*
                These are the physical memory regions detected by GRUB, we only store usable [MULTIBOOT_MEMORY_AVAILABLE] for now.
                Notes from https://wiki.osdev.org/Detecting_Memory_(x86) :
                    - (in rare/dodgy cases) may return overlapping areas
                    - These are not necessarily 4K page aligned.
                FIXME: divide overlapping memory regions.
            */
            static PhysicalMemoryManager    memory_manager;
            static bool                     instantiated;
            PhysicalMemory<MemoryRegion>    physical_memory;
            PhysicalMemory<MemoryPage>      allocated_memory_pages;
            PhysicalMemory<MemoryPage>      free_memory_pages;
    };
};
