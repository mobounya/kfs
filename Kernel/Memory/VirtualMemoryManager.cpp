#include <Kernel/Memory/VirtualMemoryManager.hpp>
#include <User/Libc/Libc.hpp>

namespace Memory
{
    VirtualMemoryManager::VirtualMemoryManager(void)
    {
        this->page_directory_size = 0;
    }

    VirtualMemoryManager::VirtualMemoryManager(void *page_tables_ptr)
    {
        // TODO: improve book keeping of all of paging structures.
        // Quick and dry way to store my pages.
        this->page_directory.page_directory_ptr = page_tables_ptr;
        this->page_directory_size = 0;

        // Setup the first 4 page directory entries to reference a page table.
        PageDirectoryEntry *pde_entry = (PageDirectoryEntry *)(page_tables_ptr);
        void *page_table_ptr = ((uint8_t *)page_tables_ptr + PAGE_TABLE_SIZE);
        for (uint8_t i = 0; i < 4; i++)
        {
            pde_entry->set_present()->set_read_write()->set_u_s()->set_pwt()->set_cache_disbled()->set_physical_address((uint32_t)page_table_ptr);
            insert_page_directory_entry(pde_entry);
            page_directory.page_table_info[i].size = 0;
            pde_entry++;
            page_table_ptr = ((uint8_t *)page_table_ptr + PAGE_TABLE_SIZE);
        }
    }

    void    *VirtualMemoryManager::allocate_virtual_memory(void *addr, uint64_t len, int prot)
    {
        // TODO: let user allocate memory at a specified virtual address.
        // TODO: let user define the protection flags.

        (void)addr;
        (void)prot;
        void *first_page_ptr = NULL;

        // We allocate by multiples of (PAGE_SIZE), so here we will round up (len) to the next multiple of (PAGE_SIZE).
        len = PhysicalMemoryManager::find_aligned_address(len, PAGE_SIZE);

        for (; len > 0; len -= PAGE_SIZE)
        {
            // Page table in current page directory entry is full, use the page table in the next page directory entry.
            if (page_directory.page_table_info[page_directory_size - 1].size >= PAGE_TABLE_SIZE)
                page_directory_size++;

            uint16_t page_directory_index = page_directory_size - 1;
            uint16_t page_table_index = page_directory.page_table_info[page_directory_index].size;

            const MemoryPage *page = memory_manager.kallocate_physical_memory_page();

            PageTableEntry pt_entry;
            pt_entry.set_present()->set_read_write()->set_u_s()->set_pwt()->set_cache_disbled()->set_physical_address(page->get_base_addr());

            PageDirectoryEntry *pde_entry = page_directory.page_directory[page_directory_index];
            PageTable          *page_table = (PageTable *)(pde_entry->page_table_address << 12);
            page_table->add_new_entry(pt_entry, page_table_index);
            page_directory.page_table_info[page_directory_index].size++;
            if (first_page_ptr == NULL)
                first_page_ptr = (void *)construct_virtual_address(page_directory_index, page_table_index, 0x0);
        }
        return (first_page_ptr);
    }

    uint32_t     VirtualMemoryManager::construct_virtual_address(uint16_t directory_index, uint16_t table_index, uint16_t offset)
    {
        uint32_t address = offset;
        address |= (uint32_t)table_index << 12;
        address |= (uint32_t)directory_index << 22;
        return address;
    }

    void VirtualMemoryManager::identity_map_memory(uint64_t virtual_address_start, uint64_t virtual_address_end)
    {
        memory_manager.reserve_physical_memory(virtual_address_start, virtual_address_end);
        for (; virtual_address_start < virtual_address_end; virtual_address_start += PAGE_SIZE)
        {
            identity_map_memory_page(virtual_address_start);
        }
    }

    void VirtualMemoryManager::load_page_directory(void)
    {
        asm volatile (
            "movl %0, %%eax\n"
            "movl %%eax, %%cr3\n"
            :: "r" (page_directory.page_directory_ptr) : "%eax"
        );
    }

    void VirtualMemoryManager::insert_page_directory_entry(PageDirectoryEntry *entry)
    {
        page_directory.add_new_entry(entry, page_directory_size);
        page_directory_size++;
    }

    void VirtualMemoryManager::identity_map_memory_page(uint64_t virtual_address)
    {
        PageDirectoryEntry *pde_entry;
        uint16_t table_index = (virtual_address & VIRTUAL_ADDRESS_TABLE_FLAG) >> 12;
        uint16_t directory_index = (virtual_address & VIRTUAL_ADDRESS_DIRECTORY_FLAG) >> 22;

        // This will not work if (directory_index) is larger than 3, since we only have memory for 4 page tables for now.
        if (directory_index < page_directory_size)
        {
            // Get directory entry that point to the corresponding page table.
            pde_entry = page_directory.page_directory[directory_index];

            // Get the address of the page table.
            uint32_t page_table_address = pde_entry->page_table_address;
            page_table_address = (page_table_address << 12); // Page address is 0x1000 (PAGE_SIZE) aligned.

            // Go to page_table_address and map the corresponding entry to (physical_address), now (virtual_address) will map to the same (physical_address)
            uint64_t physical_address = virtual_address;
            PageTable *page_table = (PageTable *)page_table_address;
            page_table->page_table[table_index].set_present()->set_read_write()->set_pwt()->set_cache_disbled()->set_physical_address(physical_address);
            page_directory.page_table_info[directory_index].size++;
        }
    }
}
