#include <Kernel/Memory/UserVirtualMemoryManager.hpp>

namespace Memory
{
    UserVirtualMemoryManager::UserVirtualMemoryManager()
    {

    }

    UserVirtualMemoryManager::UserVirtualMemoryManager(void *page_tables_ptr) : VirtualMemoryManager(page_tables_ptr)
    {

    }

    void        *UserVirtualMemoryManager::allocate_virtual_memory(void *addr, uint64_t len, int prot)
    {
        // TODO: let user allocate memory at a specified virtual address.
        // TODO: let user define the protection flags.

        (void)addr;
        (void)prot;
        void *first_page_ptr = NULL;

        // We allocate by multiples of (PAGE_SIZE), so here we will round up (len) to the next multiple of (PAGE_SIZE).
        if (len == 0)
            len = PAGE_SIZE;
        else
            len = PhysicalMemoryManager::find_aligned_address(len, PAGE_SIZE);

        for (; len > 0; len -= PAGE_SIZE)
        {
            // Page table in current page directory entry is full, use the page table in the next page directory entry.
            if (page_directory.page_table_info[page_directory_size - 1].size >= PAGE_TABLE_SIZE)
                page_directory_size++;

            uint16_t page_directory_index = page_directory_size - 1;
            uint16_t page_table_index = page_directory.page_table_info[page_directory_index].size;

            const MemoryPage *page = memory_manager.uallocate_physical_memory_page();

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

    int     UserVirtualMemoryManager::free_virtual_memory(void *addr, uint64_t len)
    {
        if (addr == NULL || PhysicalMemoryManager::find_aligned_address((uint64_t)addr, PAGE_SIZE) != (uint64_t)addr)
            return -1;

        if (len == 0)
            len = PAGE_SIZE;
        else
            len = PhysicalMemoryManager::find_aligned_address(len, PAGE_SIZE);

        for (uint8_t pages_freed = 0; pages_freed < (len / PAGE_SIZE); pages_freed++)
        {
            TranslatedLinearAddress translated_address = TranslatedLinearAddress::get_translated_address(addr);
            if (page_directory.page_directory[translated_address.page_directory_index] != NULL)
            {
                PageTable *page_table = (PageTable *)(page_directory.page_directory[translated_address.page_directory_index]->page_table_address << 12);
                uint32_t physical_address = (page_table->page_table[translated_address.page_table_index].page_table_address << 12);
                memory_manager.ufree_physical_memory_page(MemoryPage(physical_address));
                page_directory.page_table_info[translated_address.page_directory_index].size--;
            }
            addr = (void *)(((uint8_t *)addr) + PAGE_SIZE);
        }
        return 0;
    }
}
