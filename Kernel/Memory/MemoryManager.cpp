#include <Kernel/Memory/MemoryManager.hpp>
#include <User/Libc/Libc.hpp>
#include <Kernel/Multiboot/Multiboot.hpp>

namespace Memory
{
    bool            MemoryManager::instantiated = false;
    MemoryManager   MemoryManager::memory_manager;

    MemoryManager       &MemoryManager::instantiate(PageDirectory *page_directory)
    {
        if (instantiated == false)
        {
            memory_manager = MemoryManager(page_directory);
            instantiated = true;
        }
        return memory_manager;
    }

    MemoryManager::MemoryManager()
    {
        this->page_directory_size = 0;
        this->page_table_size = 0;
    }

    MemoryManager::MemoryManager(PageDirectory *page_directory) : page_directory(page_directory)
    {
        // TODO: improve book keeping of all of paging structures.
        // Quick and dry way to store my pages.
        this->page_table = (PageTable *)((uint8_t*)(page_directory) + PAGE_DIRECTORY_SIZE);
        this->page_directory_size = 0;
        this->page_table_size = 0;

        // Setup the first 4 page directory entries to reference a page table.
        for (uint8_t i = 0; i < 4; i++)
        {
            PageDirectoryEntry pde_entry;
            uint32_t page_table_ptr = (uint32_t)get_page_table_ptr();
            pde_entry.set_present()->set_read_write()->set_u_s()->set_pwt()->set_cache_disbled()->set_physical_address(page_table_ptr);
            insert_page_directory_entry(pde_entry);
            this->page_table = (PageTable *)((uint8_t*)(this->page_table) + PAGE_TABLE_SIZE);
        }

        // Reset page_table to point to the first page table.
        this->page_table = (PageTable *)((uint8_t*)(page_directory) + PAGE_DIRECTORY_SIZE);
    }

    void MemoryManager::insert_page_directory_entry(const PageDirectoryEntry &entry)
    {
        page_directory->add_new_entry(entry, page_directory_size);
        page_directory_size++;
    }

    void MemoryManager::insert_page_table_entry(const PageTableEntry &entry)
    {
        page_table->add_new_entry(entry, page_table_size);
        page_table_size++;
    }

    void MemoryManager::enable_paging(void)
    {
        // This will enable 32bit paging mode without PAE.
        asm(
            "movl %cr0, %eax\n"
            "orl $0x80000000, %eax\n"
            "movl %eax, %cr0"
        );
    }

    void MemoryManager::load_page_directory(void)
    {
        asm volatile (
            "movl %0, %%eax\n"
            "movl %%eax, %%cr3\n"
            :: "r" (page_directory) : "%eax"
        );
    }

    void MemoryManager::declare_memory_region(const MemoryRegion &region)
    {
        physical_memory.push_memory_region(region);
    }

    const MemoryPage  *MemoryManager::allocate_physical_memory_page(void)
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

    void    MemoryManager::free_physical_memory_page(const MemoryPage &page)
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

    uint64_t    MemoryManager::find_aligned_address(uint64_t address, uint64_t alignment) const
    {
        // Address is already aligned.
        if (address % alignment == 0)
            return address;
        else
            return address + (alignment - (address % alignment));
    }

    // MAYBE_FIXME: reserved physical memory are not stored anywhere, maybe store it somewhere.
    void    MemoryManager::reserve_physical_memory(uint64_t physical_address_start, uint64_t physical_address_end)
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

    const PageTable     *MemoryManager::get_page_table_ptr(void) const
    {
        return this->page_table;
    }

    void MemoryManager::identity_map_memory(uint64_t virtual_address_start, uint64_t virtual_address_end)
    {
        reserve_physical_memory(virtual_address_start, virtual_address_end);
        for (; virtual_address_start < virtual_address_end; virtual_address_start += PAGE_SIZE)
        {
            identity_map_memory_page(virtual_address_start);
        }
    }

    void MemoryManager::identity_map_memory_page(uint64_t virtual_address)
    {
        PageDirectoryEntry pde_entry;
        uint16_t table_index = (virtual_address & VIRTUAL_ADDRESS_TABLE_FLAG) >> 12;
        uint16_t directory_index = (virtual_address & VIRTUAL_ADDRESS_DIRECTORY_FLAG) >> 22;

        // This will not work if (directory_index) is larger than 3, since we only have memory for 4 page tables for now.
        if (directory_index < page_directory_size)
        {
            // Get directory entry that point to the corresponding page table.
            pde_entry = page_directory->page_directory[directory_index];

            // Get the address of the page table.
            uint32_t page_table_address = pde_entry.page_table_address;
            page_table_address = (page_table_address << 12); // Page address is 0x1000 (PAGE_SIZE) aligned.

            // Go to page_table_address and map the corresponding entry to (physical_address), now (virtual_address) will map to the same (physical_address)
            uint64_t physical_address = virtual_address;
            PageTable *page_table = (PageTable *)page_table_address;
            page_table->page_table[table_index].set_present()->set_read_write()->set_pwt()->set_cache_disbled()->set_physical_address(physical_address);
            page_table_size++;
        }
    }
}
