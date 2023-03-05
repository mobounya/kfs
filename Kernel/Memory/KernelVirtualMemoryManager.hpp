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
            void                                            *kmalloc(size_t len);
            int                                             kfree(void *addr, size_t len);
            size_t                                          ksize(const void *addr);
    };
}
