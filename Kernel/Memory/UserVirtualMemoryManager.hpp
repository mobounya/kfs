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
            void                                            *vmalloc(size_t len);
            int                                             vfree(void *addr, size_t len);
            size_t                                          vsize(const void *addr);
    };
}
