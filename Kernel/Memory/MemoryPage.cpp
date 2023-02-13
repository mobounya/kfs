#include <Kernel/Memory/MemoryPage.hpp>
#include <Kernel/Multiboot/Multiboot.hpp>

namespace Memory
{
    MemoryPage::MemoryPage(uint64_t base_addr)
    {
        MemoryRegion(base_addr, PAGE_SIZE, MULTIBOOT_MEMORY_AVAILABLE);
    }
}