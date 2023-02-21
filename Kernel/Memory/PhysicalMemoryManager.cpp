#include <Kernel/Memory/PhysicalMemoryManager.hpp>
#include <User/Libc/Libc.hpp>
#include <Kernel/Multiboot/Multiboot.hpp>

namespace Memory
{
    bool            PhysicalMemoryManager::instantiated = false;
    PhysicalMemoryManager   PhysicalMemoryManager::memory_manager;

    PhysicalMemoryManager::PhysicalMemoryManager()
    {

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

    const MemoryPage  *PhysicalMemoryManager::allocate_physical_memory_page(void)
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
            // Memory region base pointer and length will change if we have to align our page.
            uint64_t aligned_address = find_aligned_address(region.get_base_addr(), PAGE_SIZE);
            uint64_t new_length = region.get_length() - (aligned_address - region.get_base_addr());

            // Allocate new page if we have enough bytes.
            if (new_length >= PAGE_SIZE)
            {
                // Cut (PAGE_SIZE) bytes from the memory region.
                uint64_t page_base_addr = aligned_address;
                region.set_base_addr(page_base_addr + PAGE_SIZE);
                region.set_length(new_length - PAGE_SIZE);

                // Book keep the allocated page.
                MemoryPage free_page(page_base_addr);
                allocated_memory_pages.push_memory_region(free_page);
                return &(allocated_memory_pages[allocated_memory_pages.size() - 1]);
            }
        }
        return NULL;
    }

    void    PhysicalMemoryManager::free_physical_memory_page(const MemoryPage &page)
    {
        for (uint64_t i = 0; i < allocated_memory_pages.size(); i++)
        {
            if (allocated_memory_pages[i].get_base_addr() == page.get_base_addr())
            {
                free_memory_pages.push_memory_region(page);
                allocated_memory_pages.delete_memory_region(i);
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
}
