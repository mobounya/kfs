#include <Kernel/Memory/MemoryManager.hpp>
#include <Kernel/Memory/PagingStructureEntry.hpp>

namespace Memory
{
    MemoryManager::MemoryManager()
    {
        this->index = 0;
    }

    void MemoryManager::insert_page_directory_entry(PageDirectoryEntry &entry)
    {
        page_directory[index] = entry;
        index++;
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

    void MemoryManager::add_physical_memory_region(MemoryRegion &region)
    {
        physical_memory.add_memory_region(region);
    }
}
