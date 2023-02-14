#include <Kernel/Memory/MemoryManager.hpp>
#include <Kernel/Memory/PagingStructureEntry.hpp>

namespace Memory
{
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

    void MemoryManager::add_physical_memory_region(const MemoryRegion &region)
    {
        physical_memory.push_memory_region(region);
    }
}
