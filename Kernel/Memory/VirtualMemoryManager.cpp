#include <Kernel/Memory/VirtualMemoryManager.hpp>
#include <cstring.h>

namespace Memory
{
    TranslatedLinearAddress TranslatedLinearAddress::get_translated_address(const void *virtual_address)
    {
        TranslatedLinearAddress translated_address;

        uint32_t page_address = (uint32_t)virtual_address;
        translated_address.page_directory_index = (VIRTUAL_ADDRESS_DIRECTORY_FLAG & page_address) >> 22;
        translated_address.page_table_index = (VIRTUAL_ADDRESS_TABLE_FLAG & page_address) >> 12;
        translated_address.offset = (VIRTUAL_ADDRESS_OFFSET_FLAG & page_address);

        return translated_address;
    }

    VirtualMemoryManager::VirtualMemoryManager(void)
    {
        this->page_directory.page_directory_size = 0;
    }

    VirtualMemoryManager::VirtualMemoryManager(void *page_tables_ptr)
    {
        memset(&(page_directory.page_directory), 0x0, sizeof(page_directory.page_directory));

        VGA::TEXT_MODE &vga = VGA::TEXT_MODE::instantiate();

        // TODO: improve book keeping of all of paging structures.
        // Quick and dry way to store my pages.
        this->page_directory.page_directory_ptr = page_tables_ptr;
        this->page_directory.page_directory_size = 0;

        // Setup the first 4 page directory entries to reference a page table.
        PageDirectoryEntry *pde_entry = (PageDirectoryEntry *)(page_tables_ptr);
        void *page_table_ptr = ((uint8_t *)page_tables_ptr + PAGE_TABLE_SIZE);
        for (uint8_t i = 0; i < 4; i++)
        {
            pde_entry->set_present(true)->set_read_write(true)->set_u_s(true)->set_pwt(true)->set_cache_disbled(true)->set_physical_address((uint32_t)page_table_ptr);
            if (insert_page_directory_entry(pde_entry) == NULL)
            {
                vga.write_string("VirtualMemoryManager::constructor: cannot insert a new page directory entry.\n");
                return;
            }
            page_directory.page_table_info[i].size = 0;
            memset(&page_directory.page_table_info[i].entry_used, 0x0, 1024);
            pde_entry++;
            page_table_ptr = ((uint8_t *)page_table_ptr + PAGE_TABLE_SIZE);
        }
        this->page_directory.current_index = 0;
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
        /*
            Ideally you should allocate memory from the physical memory manager from a specific address
            and then map that address to the same virtual address, but for now we don't have the ability to allocate
            physical memory at a specified address.
            TODO: allocate the memory rather than calling (reserve_physical_memory) which just removes the memory of the available memory pool.
        */
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

    const PagingStructureEntry  *VirtualMemoryManager::insert_page_directory_entry(PageDirectoryEntry *entry)
    {
        const PagingStructureEntry *ret_entry = page_directory.add_new_entry(entry, this->page_directory.page_directory_size);
        if (ret_entry == NULL)
            return NULL;
        else {
            this->page_directory.page_directory_size++;
            return ret_entry;
        }
    }

    void VirtualMemoryManager::identity_map_memory_page(uint64_t virtual_address)
    {
        PageDirectoryEntry *pde_entry;
        uint16_t table_index = (virtual_address & VIRTUAL_ADDRESS_TABLE_FLAG) >> 12;
        uint16_t directory_index = (virtual_address & VIRTUAL_ADDRESS_DIRECTORY_FLAG) >> 22;

        // This will not work if (directory_index) is larger than 3, since we only have memory for 4 page tables for now.
        if (directory_index < this->page_directory.page_directory_size)
        {
            // Get directory entry that point to the corresponding page table.
            pde_entry = page_directory.page_directory[directory_index];

            // Get the address of the page table.
            uint32_t page_table_address = pde_entry->physical_address;
            page_table_address = (page_table_address << 12); // Page address is 0x1000 (PAGE_SIZE) aligned.

            // Go to page_table_address and map the corresponding entry to (physical_address), now (virtual_address) will map to the same (physical_address)
            uint64_t physical_address = virtual_address;
            PageTable *page_table = (PageTable *)page_table_address;
            page_table->page_table[table_index].set_present(true)->set_read_write(true)->set_pwt(true)->set_cache_disbled(true)->set_physical_address(physical_address);
            page_directory.page_table_info[directory_index].size++;
        } else 
        {
            VGA::TEXT_MODE &vga = VGA::TEXT_MODE::instantiate();
            vga.write_string("VirtualMemoryManager::identity_map_memory_page: cannot access page directory index (");
            vga.write_string(itoa(directory_index));
            vga.write_string(").\n");
        }
    }

    int    VirtualMemoryManager::disable_page(const void *virtual_address, uint32_t len)
    {
        VGA::TEXT_MODE &vga = VGA::TEXT_MODE::instantiate();

        if (PhysicalMemoryManager::find_aligned_address((uint64_t)virtual_address, PAGE_SIZE) != (uint64_t)virtual_address)
        {
            vga.write_string("VirtualMemoryManager::disable_page: Virtual address is not page aligned\n");
            return -1;
        }

        if (len == 0)
            len = PAGE_SIZE;
        else
            len = PhysicalMemoryManager::find_aligned_address(len, PAGE_SIZE);

        for (uint32_t n_pages = 0; n_pages < (len / PAGE_SIZE); n_pages++)
        {
            TranslatedLinearAddress address = TranslatedLinearAddress::get_translated_address(((uint8_t *)virtual_address) + (n_pages * PAGE_SIZE));
            if (page_directory.page_directory[address.page_directory_index] == NULL)
            {
                vga.write_string("VirtualMemoryManager::disable_page: cannot access page directory index (");
                vga.write_string(itoa(address.page_directory_index));
                vga.write_string(").\n");
                return -1;
            }
            PageTable *page_table = (PageTable *)(page_directory.page_directory[address.page_directory_index]->physical_address << 12);
            PageTableEntry pte;
            pte.set_present(false)->set_read_write(false);
            page_table->add_new_entry(pte, address.page_table_index);
            if (page_directory.page_table_info[address.page_directory_index].entry_used[address.page_table_index] == false)
                page_directory.page_table_info[address.page_directory_index].size++;
            page_directory.page_table_info[address.page_directory_index].entry_used[address.page_table_index] = true;
        }
        return 0;
    }
}
