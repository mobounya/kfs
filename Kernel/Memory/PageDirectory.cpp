#include <Kernel/Memory/PageDirectory.hpp>

#include <stdint.h>

namespace Memory
{
    const PagingStructureEntry *PageDirectory::add_new_entry(PagingStructureEntry *entry, uint16_t index)
    {
        if (this->page_directory[index] != NULL)
            return NULL;
        this->page_directory[index] = entry;
        return (this->page_directory[index]);
    }
}
