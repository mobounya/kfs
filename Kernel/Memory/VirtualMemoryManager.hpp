#pragma once

#include <stdint.h>

#include <Kernel/Memory/PhysicalMemoryManager.hpp>
#include <Kernel/Memory/VirtualAddress.hpp>

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

        static TranslatedLinearAddress get_translated_address(const VirtualAddress &addr);
    };

    class VirtualMemoryManager
    {
        public:
            VirtualMemoryManager();
            VirtualMemoryManager(void *page_tables_ptr);

        public:
            static VirtualAddress       construct_virtual_address(uint16_t directory_index, uint16_t table_index, uint16_t offset);
            void                        load_page_directory(void);
            const PagingStructureEntry  *insert_page_directory_entry(PagingStructureEntry *entry);
            void                        identity_map_memory(VirtualAddress start, const VirtualAddress &end);
            int                         disable_page(const VirtualAddress &addr, uint32_t len);

        private:
            void                    identity_map_memory_page(const VirtualAddress &page);
            void                    find_free_page_table_entries(uint16_t number_of_pages, uint16_t &page_directory_index, uint16_t &page_table_index);

        protected:
            PhysicalMemoryManager &memory_manager = PhysicalMemoryManager::instantiate();
            PageDirectory         page_directory;
    };
}
