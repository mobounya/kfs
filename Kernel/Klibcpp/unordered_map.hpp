#pragma once

#include <stdint.h>
#include <cstring.h>

#include <Kernel/Memory/QuickDirtyMalloc.hpp>

namespace std
{
    // TODO: you need to allocate memory size == capacity.
    template <class Key, class T>
    class unordered_map
    {
        typedef Key                          key_type;
        typedef T                            mapped_type;
        typedef int                          size_type;

        private:
            Key         *m_first;
            T           *m_second;
            size_type   m_size;
            size_type   m_capacity;

        private:
            size_type get_index_of_key(const Key& key) const
            {
                for (size_type i = 0; i < m_size; i++)
                {
                    if (m_first[i] == key)
                        return i;
                }
                return -1;
            }

        public:
            unordered_map()
            {
                m_first = (Key *)quick_dirty_kmalloc(sizeof(Key) * 15);
                m_second = (T *)quick_dirty_kmalloc(sizeof(T) * 15);
                m_size = 0;
                m_capacity = 15;
            }

            T& operator[](const Key& key)
            {
                T temp;
                size_type key_index = get_index_of_key(key);
                if (key_index == -1)
                {
                    m_first[m_size] = key;
                    m_second[m_size] = temp;
                    m_size++;
                    return m_second[m_size - 1];
                }
                return m_second[key_index];
            }

            T& at(const Key& key)
            {
                T temp;
                size_type key_index = get_index_of_key(key);
                if (key_index == -1)
                    return m_second[m_size];
                else
                    return m_second[key_index];
            }

            bool empty() const
            {
                return (m_size == 0);
            }

            size_type size() const
            {
                return m_size;
            }

            size_type max_size() const
            {
                return m_size;
            }

            void clear()
            {
                Key ktemp;
                T   vtemp;
                for (size_type i = 0; i < m_size; i++)
                {
                    m_first[i] = ktemp;
                    m_second[i] = vtemp;
                }
                m_size = 0;
            }

            size_type count(const Key& key) const
            {
                size_type key_index = get_index_of_key(key);
                if (key_index == -1)
                    return 0;
                else
                    return 1;
            }

            bool contains(const Key& key) const
            {
                return (count(key) == 1);
            }
    };
}
