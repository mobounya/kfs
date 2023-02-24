#include <Kernel/Memory/PhysicalMemoryManager.hpp>
#include <Kernel/Multiboot/Multiboot.hpp>
#include <string.h>

namespace Memory
{
    bool                                           PhysicalMemoryManager::instantiated = false;
    PhysicalMemoryManager                          PhysicalMemoryManager::memory_manager;

    PhysicalMemoryManager::MemoryPool::MemoryPool()
    {

    }

    PhysicalMemoryManager::MemoryPool::MemoryPool(uint64_t base_addr, uint64_t end_addr) : MemoryRegion(base_addr, end_addr - base_addr, MULTIBOOT_MEMORY_AVAILABLE)
    {
        this->end_addr = end_addr;
    }

    PhysicalMemoryManager::PhysicalMemoryManager()
    {
        kernel_space = PhysicalMemoryManager::MemoryPool(0x400000, 0x800000);
        user_space = PhysicalMemoryManager::MemoryPool(0x800000, 0x1000000);
    }

    PhysicalMemoryManager       &PhysicalMemoryManager::instantiate()
    {
        if (instantiated == false)
        {
            memory_manager = PhysicalMemoryManager();
            instantiated = true;
        }
        return memory_manager;
    }

    void PhysicalMemoryManager::enable_paging(void)
    {
        // This will enable 32bit paging mode without PAE.
        asm(
            "movl %cr0, %eax\n"
            "orl $0x80000000, %eax\n"
            "movl %eax, %cr0"
        );
    }

    void PhysicalMemoryManager::declare_memory_region(const MemoryRegion &region)
    {
        physical_memory.push_memory_region(region);
    }

    const MemoryPage    *PhysicalMemoryManager::kallocate_physical_memory_page(void)
    {
        return this->alloc(kernel_space, kernel_allocated_memory_pages, kernel_free_memory_pages);
    }

    const MemoryPage    *PhysicalMemoryManager::uallocate_physical_memory_page(void)
    {
        return this->alloc(user_space, user_allocated_memory_pages, user_free_memory_pages);
    }

    /*
        pool:
            - We will try to allocate memory that happens to be in this area, we still need check to see if it is available for allocation.
        allocated_memory_pages: 
            - Keep track of allocated pages, push to it when you allocate a memory page.
        free_memory_pages:
            - Keep track of free pages, try to allocate from it first.
    */
    const MemoryPage  *PhysicalMemoryManager::alloc(const MemoryPool &pool, PhysicalMemory<MemoryPage> &allocated_memory_pages, PhysicalMemory<MemoryPage> &free_memory_pages)
    {
        if (free_memory_pages.size() > 0)
        {
            const MemoryPage *page = &(free_memory_pages[free_memory_pages.size() - 1]);
            allocated_memory_pages.push_memory_region(*page);
            free_memory_pages.delete_memory_region(free_memory_pages.size() - 1);
            return page;
        }

        for (uint32_t i = 0; i < physical_memory.size(); i++)
        {
            MemoryRegion region = physical_memory[i];

            // Memory region base pointer and length will change if we have to page align the region.
            uint64_t aligned_address = find_aligned_address(region.get_base_addr(), PAGE_SIZE);
            uint64_t new_length = region.get_length() - (aligned_address - region.get_base_addr());
            region.set_base_addr(aligned_address);
            region.set_length(new_length);

            // Check if this region of memory or part of it is in our wanted pool.
            MemoryRegion available_memory = get_overlapping_memory_region(pool, region);

            // Check if there's PAGE_SIZE available in the pool.
            if (available_memory.get_length() >= PAGE_SIZE)
            {
                /*
                    Let's cut this region into 3 parts if available:
                        - Memory before our page. (give it back)
                        - Our page. (allocate and return)
                        - Memory after our page. (give it back)
                */

                // This the size of the memory before the page that we will cut away.
                uint64_t first_hole_size = available_memory.get_base_addr() - region.get_base_addr();

                // This the size of the memory after the page that we will cut away.
                uint64_t second_hole_size = (region.get_base_addr() + region.get_length()) - (available_memory.get_base_addr() + PAGE_SIZE);

                // Give back the first memory hole.
                if (first_hole_size > 0)
                {
                    MemoryRegion hole(region.get_base_addr(), first_hole_size, MULTIBOOT_MEMORY_AVAILABLE);
                    physical_memory.push_memory_region(hole);
                }

                // Give back the second memory hole.
                if (second_hole_size > 0)
                {
                    MemoryRegion hole(available_memory.get_base_addr() + PAGE_SIZE, second_hole_size, MULTIBOOT_MEMORY_AVAILABLE);
                    physical_memory.push_memory_region(hole);
                }

                // Allocate page.
                MemoryPage allocated_page(available_memory.get_base_addr());
                allocated_memory_pages.push_memory_region(allocated_page);

                // by now this region is split into 3 and given back, so delete it.
                physical_memory.delete_memory_region(i);

                return &(allocated_memory_pages[allocated_memory_pages.size() - 1]);
            }
        }
        return NULL;
    }

    MemoryRegion    PhysicalMemoryManager::get_overlapping_memory_region(const MemoryRegion &region_1, const MemoryRegion &region_2)
    {
        uint64_t region_1_end_ptr = region_1.get_base_addr() + region_1.get_length();
        uint64_t region_2_end_ptr = region_2.get_base_addr() + region_2.get_length();
        uint32_t overlapping_start = 0;
        uint32_t overlapping_end = 0;

        if (region_2.get_base_addr() >= region_1.get_base_addr() && region_2.get_base_addr() < region_1_end_ptr)
        {
            overlapping_start = region_2.get_base_addr();
            overlapping_end = (region_2_end_ptr >= region_1_end_ptr) ? region_1_end_ptr : region_2_end_ptr;
        }
        else if (region_1.get_base_addr() >= region_2.get_base_addr() && region_1.get_base_addr() < region_2_end_ptr)
        {
            overlapping_start = region_1.get_base_addr();
            overlapping_end = (region_2_end_ptr >= region_1_end_ptr) ? region_1_end_ptr : region_2_end_ptr;
        }
        return MemoryRegion(overlapping_start, overlapping_end - overlapping_start, MULTIBOOT_MEMORY_AVAILABLE);
    }

    void    PhysicalMemoryManager::kfree_physical_memory_page(const MemoryPage &page)
    {
        for (uint64_t i = 0; i < kernel_allocated_memory_pages.size(); i++)
        {
            if (kernel_allocated_memory_pages[i].get_base_addr() == page.get_base_addr())
            {
                kernel_free_memory_pages.push_memory_region(page);
                kernel_allocated_memory_pages.delete_memory_region(i);
            }
        }
    }

    void    PhysicalMemoryManager::ufree_physical_memory_page(const MemoryPage &page)
    {
        for (uint32_t i = 0; i < user_allocated_memory_pages.size(); i++)
        {
            if (user_allocated_memory_pages[i].get_base_addr() == page.get_base_addr())
            {
                user_free_memory_pages.push_memory_region(page);
                user_allocated_memory_pages.delete_memory_region(i);
            }
        }
    }

    uint64_t    PhysicalMemoryManager::find_aligned_address(uint64_t address, uint64_t alignment)
    {
        // Address is already aligned.
        if (address % alignment == 0)
            return address;
        else
            return address + (alignment - (address % alignment));
    }

    // MAYBE_FIXME: reserved physical memory are not stored anywhere, maybe store it somewhere.
    void    PhysicalMemoryManager::reserve_physical_memory(uint64_t physical_address_start, uint64_t physical_address_end)
    {
        for (uint32_t i = 0; i < physical_memory.size(); i++)
        {
            const MemoryRegion &region = physical_memory[i];

            uint64_t base_ptr = region.get_base_addr();
            uint64_t length = region.get_length();

            if (physical_address_start >= base_ptr && physical_address_start < (base_ptr + length))
            {
                int64_t first_region_length = physical_address_start - base_ptr;
                int64_t second_region_length = (base_ptr + length) - physical_address_end;
                
                if (first_region_length > 0)
                {
                    MemoryRegion region(base_ptr, first_region_length, MULTIBOOT_MEMORY_AVAILABLE);
                    physical_memory.push_memory_region(region);
                }
                if (second_region_length > 0)
                {
                    MemoryRegion region(physical_address_end, second_region_length, MULTIBOOT_MEMORY_AVAILABLE);
                    physical_memory.push_memory_region(region);
                }

                physical_memory.delete_memory_region(i);
                if (first_region_length > 0 && second_region_length > 0)
                    break;
            }
        }
    }

    void    PhysicalMemoryManager::print_physical_memory_regions(VGA::TEXT_MODE &vga) const
    {
        for (uint32_t i = 0; i < physical_memory.size(); i++)
        {
            vga.write_string("-------- ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            vga.write_string("Available region #", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            vga.write_string(itoa(i), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            vga.write_string(" --------\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            vga.write_string("Base address: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            vga.write_string(itoa(physical_memory[i].get_base_addr()), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            vga.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            vga.write_string("Length: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            vga.write_string(itoa(physical_memory[i].get_length()), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            vga.write_string("\n\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
        }
    }

    void    PhysicalMemoryManager::print_kallocated_memory_pages(VGA::TEXT_MODE &vga, uint8_t max) const
    {
        if (kernel_allocated_memory_pages.size() == 0)
        {
            vga.write_string("No kernel allocated memory !\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
        }
        else
        {
            max = (max == 0) ? kernel_allocated_memory_pages.size() : max;
            for (uint32_t i = 0; i < max && i < kernel_allocated_memory_pages.size(); i++)
            {
                vga.write_string("-------- ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Allocated page #", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(i), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(" (Kernel) --------\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Base address: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(kernel_allocated_memory_pages[i].get_base_addr()), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Length: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(kernel_allocated_memory_pages[i].get_length()), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("\n\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            }
        }
    }

    void    PhysicalMemoryManager::print_uallocated_memory_pages(VGA::TEXT_MODE &vga, uint8_t max) const
    {
        if (user_allocated_memory_pages.size() == 0)
        {
            vga.write_string("No user allocated memory !\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
        }
        else
        {
            max = (max == 0) ? user_allocated_memory_pages.size() : max;
            for (uint32_t i = 0; i < max && i < user_allocated_memory_pages.size(); i++)
            {
                vga.write_string("-------- ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Allocated page #", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(i), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(" (User) --------\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Base address: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(user_allocated_memory_pages[i].get_base_addr()), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Length: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(user_allocated_memory_pages[i].get_length()), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("\n\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            }
        }
    }

    void    PhysicalMemoryManager::print_kfree_memory_pages(VGA::TEXT_MODE &vga, uint8_t max) const
    {
        if (kernel_free_memory_pages.size() == 0)
        {
             vga.write_string("No kernel free pages available !\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
        }
        else
        {
            max = (max == 0) ? kernel_free_memory_pages.size() : max;
            for (uint32_t i = 0; i < max && i < kernel_free_memory_pages.size(); i++)
            {
                vga.write_string("-------- ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Free page #", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(i), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(" (Kernel) --------\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Base address: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(kernel_free_memory_pages[i].get_base_addr()), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Length: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(kernel_free_memory_pages[i].get_length()), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("\n\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            }
        }
    }

    void    PhysicalMemoryManager::print_ufree_memory_pages(VGA::TEXT_MODE &vga, uint8_t max) const
    {
        if (user_free_memory_pages.size() == 0)
        {
             vga.write_string("No kernel free pages available !\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
        }
        else
        {
            max = (max == 0) ? user_free_memory_pages.size() : max;
            for (uint32_t i = 0; i < max && i < user_free_memory_pages.size(); i++)
            {
                vga.write_string("-------- ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Free page #", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(i), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(" (Kernel) --------\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Base address: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(user_free_memory_pages[i].get_base_addr()), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("Length: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string(itoa(user_free_memory_pages[i].get_length()), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                vga.write_string("\n\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            }
        }
    }
}
