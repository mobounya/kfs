#pragma once

#include <User/c++/vector.hpp>

namespace Memory
{
    template <typename T>
    class PhysicalMemory
    {
        typedef const T&    const_reference;
        typedef uint32_t    size_type;
        typedef uint64_t    size_t;

        private:
            std::vector<T> memory_regions;

        public:
            const_reference    operator[](size_type index) const
            {
                return memory_regions[index];
            }

            void    push_memory_region(const_reference region)
            {
                memory_regions.push_back(region);
            }

            T*      delete_memory_region(size_type index) const
            {
                return memory_regions.erase(index);
            }

            size_type size() const
            {
                return memory_regions.size();
            }
    };
}
