#pragma once

#include <stdint.h>

#include <Kernel/Memory/PhysicalMemoryManager.hpp>

#define VIRTUAL_ADDRESS_OFFSET_FLAG 0xFFF
#define VIRTUAL_ADDRESS_TABLE_FLAG 0x3FF000
#define VIRTUAL_ADDRESS_DIRECTORY_FLAG 0xFFC00000

namespace Memory
{
    class VirtualMemoryManager
    {
        public:
            VirtualMemoryManager();
            VirtualMemoryManager(void *page_tables_ptr);

        public:
            void                *allocate_virtual_memory(void *addr, uint64_t len, int prot);
            static uint32_t     construct_virtual_address(uint16_t directory_index, uint16_t table_index, uint16_t offset);
            void                load_page_directory(void);
            void                insert_page_directory_entry(PagingStructureEntry *entry);
            void                identity_map_memory(uint64_t virtual_address_start, uint64_t virtual_address_end);

        private:
            void                 identity_map_memory_page(uint64_t virtual_address);

        private:
            PhysicalMemoryManager &memory_manager = PhysicalMemoryManager::instantiate();
            PageDirectory         page_directory;
            uint16_t              page_directory_size;
    };
}
