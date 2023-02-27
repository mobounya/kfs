#pragma once

#include <Kernel/Memory/VirtualMemoryManager.hpp>

namespace Memory
{
    class UserVirtualMemoryManager : public VirtualMemoryManager
    {
        public:
            UserVirtualMemoryManager();
            UserVirtualMemoryManager(void *page_tables_ptr);

        public:
            void                                            *allocate_virtual_memory(void *addr, uint64_t len, int prot);
            int                                             free_virtual_memory(void *addr, uint64_t len);
    };
}
