#pragma once

#include <Kernel/Memory/VirtualMemoryManager.hpp>

namespace Memory
{
    class KernelVirtualMemoryManager : public VirtualMemoryManager
    {
        public:
            KernelVirtualMemoryManager();
            KernelVirtualMemoryManager(void *page_tables_ptr);

        public:
            void                                            *allocate_virtual_memory(void *addr, uint64_t len, int prot);
            int                                             free_virtual_memory(void *addr, uint64_t len);
    };
}
