#include <Kernel/Memory/PageDirectory.hpp>

#include <stdint.h>

namespace Memory
{
    void PageDirectory::add_new_entry(const PagingStructureEntry &entry, uint16_t index)
    {
        this->page_directory[index] = entry;
    }
}
