#pragma once

#include <stdint.h>

#include <Kernel/Memory/PhysicalMemoryManager.hpp>

#define VIRTUAL_ADDRESS_OFFSET_FLAG 0xFFF
#define VIRTUAL_ADDRESS_TABLE_FLAG 0x3FF000
#define VIRTUAL_ADDRESS_DIRECTORY_FLAG 0xFFC00000

namespace Memory
{
    struct TranslatedLinearAddress
    {
        uint32_t page_directory_index : 10;
        uint32_t page_table_index : 10;
        uint32_t offset : 12;

        static TranslatedLinearAddress get_translated_address(const void *virtual_address);
    };

    class VirtualMemoryManager
    {
        public:
            VirtualMemoryManager();
            VirtualMemoryManager(void *page_tables_ptr);

        public:
            static uint32_t             construct_virtual_address(uint16_t directory_index, uint16_t table_index, uint16_t offset);
            void                        load_page_directory(void);
            const PagingStructureEntry  *insert_page_directory_entry(PagingStructureEntry *entry);
            void                        identity_map_memory(uint64_t virtual_address_start, uint64_t virtual_address_end);
            int                         disable_page(const void *virtual_address, uint32_t len);

        private:
            void                    identity_map_memory_page(uint64_t virtual_address);
            void                    find_free_page_table_entries(uint16_t number_of_pages, uint16_t &page_directory_index, uint16_t &page_table_index);

        protected:
            PhysicalMemoryManager &memory_manager = PhysicalMemoryManager::instantiate();
            PageDirectory         page_directory;
    };
}
