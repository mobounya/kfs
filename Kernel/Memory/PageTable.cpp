#include <Kernel/Memory/PageTable.hpp>

namespace Memory
{
    void PageTable::add_new_entry(const PageTableEntry &entry, uint16_t index)
    {
        page_table[index] = entry;
    }
}
