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
        class MemoryPool : public MemoryRegion
        {
            public:
                MemoryPool();
                MemoryPool(uint64_t base_addr, uint64_t end_addr);

            public:
                void        set_end_addr(uint64_t end_addr);
                uint64_t    get_end_addr(void) const;

            private:
                uint64_t end_addr;
        };

        private:
            PhysicalMemoryManager();

        public:
            static PhysicalMemoryManager       &instantiate(void);
            void                                enable_paging(void);
            void                                declare_memory_region(const MemoryRegion &region);
            const MemoryPage                    *kallocate_physical_memory_page(void);
            void                                kfree_physical_memory_page(const MemoryPage &page);
            static uint64_t                     find_aligned_address(uint64_t address, uint64_t alignment);
            void                                reserve_physical_memory(uint64_t physical_address_start, uint64_t physical_address_end);
            void                                print_physical_memory_regions(VGA::TEXT_MODE &vga);
            void                                print_kallocated_memory_pages(VGA::TEXT_MODE &vga);
            void                                print_kfree_memory_pages(VGA::TEXT_MODE &vga);
            static MemoryRegion                 get_overlapping_memory_region(const MemoryRegion &region_1, const MemoryRegion &region_2);

        private:
            const MemoryPage                    *alloc(const MemoryPool &pool, PhysicalMemory<MemoryPage> &allocated_memory_pages, PhysicalMemory<MemoryPage> &free_memory_pages);

            /*
                These are the physical memory regions detected by GRUB, we only store usable [MULTIBOOT_MEMORY_AVAILABLE] for now.
                Notes from https://wiki.osdev.org/Detecting_Memory_(x86) :
                    - (in rare/dodgy cases) may return overlapping areas
                    - These are not necessarily 4K page aligned.
                FIXME: divide overlapping memory regions.
            */
            PhysicalMemory<MemoryRegion>    physical_memory;
            PhysicalMemory<MemoryPage>      kernel_allocated_memory_pages;
            PhysicalMemory<MemoryPage>      kernel_free_memory_pages;

            MemoryPool kernel_space;
            MemoryPool user_space;

            static PhysicalMemoryManager    memory_manager;
            static bool                     instantiated;
    };
};
