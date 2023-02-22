#include <Kernel/Memory/PageTable.hpp>

namespace Memory
{
    const PageTableEntry    &PageTable::add_new_entry(const PageTableEntry &entry, uint16_t index)
    {
        page_table[index] = entry;
        return page_table[index];
    }
}
