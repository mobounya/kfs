#include <Kernel/Memory/PageDirectory.hpp>

#include <stdint.h>
#include <Kernel/VGA/VGA.hpp>
#include <string.h>

namespace Memory
{
    const PagingStructureEntry *PageDirectory::add_new_entry(PagingStructureEntry *entry, uint16_t index)
    {
        if (this->page_directory[index] != NULL)
            return NULL;
        this->page_directory[index] = entry;
        return (this->page_directory[index]);
    }

    int16_t                    PageDirectory::find_free_page_table(uint16_t n_entries, uint16_t offset)
    {
        uint16_t free_page_table_index = offset;

        for (; free_page_table_index < page_directory_size; free_page_table_index++)
        {
            uint16_t empty_entries = N_PAGE_TABLE_ENTRIES - page_table_info[free_page_table_index].size;
            if (empty_entries >= n_entries)
                return free_page_table_index;
        }

        return -1;
    }

    // FIXME: this will look for (number_of_pages) contiguous empty entries in the same page table,
    // however we can use contiguous empty entries from two different (contiguous) page tables.
    bool        PageDirectory::find_contiguous_free_pages(uint16_t number_of_pages, uint16_t &page_directory_index, uint16_t &page_table_index)
    {
        uint16_t offset = 0;
        int16_t free_page_table_index;

        // Find (number_of_pages) contiguous empty entries.
        while ((free_page_table_index = find_free_page_table(number_of_pages, offset)) != -1)
        {
            offset = free_page_table_index + 1;
            int16_t first_entry = -1;
            uint16_t n_pages_found = 0;

            for (uint16_t i = 0; i < N_PAGE_DIRECTORY_ENTRIES; i++)
            {
                if (n_pages_found == number_of_pages)
                    break;
                if (page_table_info[free_page_table_index].entry_used[i] == false)
                {
                    n_pages_found++;
                    if (first_entry == -1)
                        first_entry = i;
                }
                else
                {
                    first_entry = -1;
                    n_pages_found = 0;
                }
            }

            if (n_pages_found == number_of_pages)
            {
                page_directory_index = free_page_table_index;
                page_table_index = first_entry;
                return true;
            }
        }
        return false;
    }
}
