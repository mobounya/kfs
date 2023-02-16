#include <Kernel/Memory/MemoryManager.hpp>
#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/VGA/VGA.hpp>
#include <User/Libc/Libc.hpp>

namespace Memory
{
    MemoryManager::MemoryManager()
    {

    }

    MemoryManager::MemoryManager(PageDirectory *page_directory) : page_directory(page_directory)
    {
        // TODO: improve book keeping of all of paging structures.
        // Quick and dry way to store my pages.
        this->page_table = (PageTable *)((uint8_t*)(page_directory) + PAGE_DIRECTORY_SIZE);
        this->page_directory_size = 0;
        this->page_table_size = 0;
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
            "mov %cr0, %eax\n"
            "or $0x80000000, %eax\n"
            "mov %eax, %cr0"
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

    uint64_t    MemoryManager::find_aligned_address(uint64_t address, uint64_t alignment) const
    {
        // Address is already aligned.
        if (address % alignment == 0)
            return address;
        else
            return address + (alignment - (address % alignment));
    }

    const MemoryPage  *MemoryManager::allocate_memory_page(void)
    {
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

    const PageTable     *MemoryManager::get_page_table_ptr(void) const
    {
        return this->page_table;
    }
}
