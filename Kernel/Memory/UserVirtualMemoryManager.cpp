#include <Kernel/Memory/UserVirtualMemoryManager.hpp>
#include <string.h>

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
        VGA::TEXT_MODE &vga = VGA::TEXT_MODE::instantiate();

        (void)addr;
        (void)prot;
        void *first_page_ptr = NULL;

        // We allocate by multiples of (PAGE_SIZE), so here we will round up (len) to the next multiple of (PAGE_SIZE).
        if (len == 0)
            len = PAGE_SIZE;
        else
            len = PhysicalMemoryManager::find_aligned_address(len, PAGE_SIZE);

        uint16_t page_directory_index;
        uint16_t page_table_index;
    
        if (page_directory.find_contiguous_free_pages(len / PAGE_SIZE, page_directory_index, page_table_index) == false)
        {
            vga.write_string("UserVirtualMemoryManager::allocate_virtual_memory: failed to allocate virtual memory page.\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            return NULL;
        }

        for (; len > 0; len -= PAGE_SIZE)
        {
            const MemoryPage *page = memory_manager.uallocate_physical_memory_page();
            if (page == NULL)
            {
                vga.write_string("UserVirtualMemoryManager::allocate_virtual_memory: failed to allocate physical memory page.\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                return NULL;
            }

            PageDirectoryEntry *pde_entry = page_directory.page_directory[page_directory_index];
            if (pde_entry == NULL)
            {
                vga.write_string("UserVirtualMemoryManager::allocate_virtual_memory: such page directory.\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
                return NULL;
            }
            PageTable          *page_table = (PageTable *)(pde_entry->physical_address << 12);

            PageTableEntry pt_entry;
            pt_entry.set_present(true)->set_read_write(true)->set_u_s(true)->set_pwt(true)->set_cache_disbled(true)->set_physical_address(page->get_base_addr());

            page_table->add_new_entry(pt_entry, page_table_index);
            if (first_page_ptr == NULL)
                first_page_ptr = (void *)construct_virtual_address(page_directory_index, page_table_index, 0x0);
            page_directory.page_table_info[page_directory_index].entry_used[page_table_index] = true;
            page_directory.page_table_info[page_directory_index].size++;
            page_table_index++;
        }
        return (first_page_ptr);
    }

    int     UserVirtualMemoryManager::free_virtual_memory(void *addr, uint64_t len)
    {
        VGA::TEXT_MODE &vga = VGA::TEXT_MODE::instantiate();

        if (addr == NULL || PhysicalMemoryManager::find_aligned_address((uint64_t)addr, PAGE_SIZE) != (uint64_t)addr)
        {
            vga.write_string("UserVirtualMemoryManager::free_virtual_memory: Virtual address is not page aligned\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
            return -1;
        }

        if (len == 0)
            len = PAGE_SIZE;
        else
            len = PhysicalMemoryManager::find_aligned_address(len, PAGE_SIZE);

        for (uint8_t pages_freed = 0; pages_freed < (len / PAGE_SIZE); pages_freed++)
        {
            TranslatedLinearAddress translated_address = TranslatedLinearAddress::get_translated_address(addr);
            if (page_directory.page_directory[translated_address.page_directory_index] != NULL)
            {
                if (page_directory.page_table_info[translated_address.page_directory_index].entry_used[translated_address.page_table_index] == true)
                {
                    PageTable *page_table = (PageTable *)(page_directory.page_directory[translated_address.page_directory_index]->physical_address << 12);
                    uint32_t physical_address = (page_table->page_table[translated_address.page_table_index].physical_address << 12);
                    memory_manager.ufree_physical_memory_page(MemoryPage(physical_address));
                    page_directory.page_table_info[translated_address.page_directory_index].entry_used[translated_address.page_table_index] = false;
                    page_directory.page_table_info[translated_address.page_directory_index].size--;
                    memset(&(page_table->page_table[translated_address.page_table_index]), 0x0, sizeof(PageTableEntry));
                }
            }
            addr = (void *)(((uint8_t *)addr) + PAGE_SIZE);
        }

        return 0;
    }
}
