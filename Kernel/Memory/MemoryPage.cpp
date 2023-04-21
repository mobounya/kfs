#include <Kernel/Memory/MemoryPage.hpp>
#include <Kernel/Multiboot/Multiboot.hpp>

namespace Memory
{
    MemoryPage::MemoryPage(void)
    {

    }

    MemoryPage::MemoryPage(uint32_t base_addr) : MemoryRegion(base_addr, PAGE_SIZE, MULTIBOOT_MEMORY_AVAILABLE)
    {
    }
}
