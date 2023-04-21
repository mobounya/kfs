#pragma once

#include <stdint.h>
#include <stddef.h>

#include <Kernel/Memory/QuickDirtyMalloc.hpp>

namespace std
{
    template <typename T>
    class vector
    {
        typedef T                            value_type;
        typedef size_t                       size_type;
        typedef value_type&                  reference;
        typedef const value_type&            const_reference;
        typedef value_type*                  pointer;
        typedef const value_type*            const_pointer;

        private:
            pointer     m_data;
            size_type   m_size;
            size_type   m_capacity;

        public:
            vector()
            {
                m_capacity = 25;
                m_data = (pointer)quick_dirty_kmalloc(sizeof(T) * m_capacity);
                m_size = 0;
            }

            reference at(size_type pos)
            {
                return m_data[pos];
            }

            const_reference at(size_type pos) const
            {
                return m_data[pos];
            }

            reference operator[](size_type pos)
            {
                return m_data[pos];
            }

            const_reference operator[](size_type pos) const
            {
                return m_data[pos];
            }

            reference front()
            {
                return m_data[0];
            }

            const_reference front() const
            {
                return m_data[0];
            }

            reference back()
            {
                if (m_size > 0)
                    return m_data[m_size - 1];
                else
                    return m_data[0];
            }

            const_reference back() const
            {
                if (m_size > 0)
                    return m_data[m_size - 1];
                else
                    return m_data[0];
            }

            pointer data()
            {
                return m_data;
            }

            const_pointer data() const
            {
                return m_data;
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

            void reserve(size_type new_cap)
            {
                if (new_cap <= m_capacity || new_cap <= m_size)
                    return ;
                pointer temp = (pointer)quick_dirty_kmalloc(sizeof(value_type) * new_cap);
                m_capacity = get_actual_allocated_size(sizeof(value_type) * new_cap) / sizeof(value_type);
                for (size_type i = 0; i < m_size; i++)
                    temp[i] = m_data[i];
                quick_dirty_kfree(m_data);
                m_data = temp;
            }

            size_type capacity() const
            {
                return m_capacity;
            }

            void clear()
            {
                memset(m_data, 0, m_capacity * sizeof(value_type));
                m_size = 0;
            }

            pointer insert(size_type index, const T& value)
            {
                if (index >= m_capacity)
                    reserve(index + 1);
                m_data[index] = value;
                if (index >= m_size)
                    m_size = index + 1;
                return (m_data + index);
            }

            pointer erase(size_type pos)
            {
                if (pos > m_size)
                    return NULL;
                for (size_type i = (pos + 1); i < m_size; i++)
                    m_data[i - 1] = m_data[i];
                m_size--;
                return (m_data + pos);
            }

            void        push_back(const T& value)
            {
                if (m_size >= capacity())
                    reserve(m_capacity + 1);
                m_data[m_size] = value;
                m_size++;
            }

            void        pop_back()
            {
                m_size--;
                memset(m_data + m_size, 0, sizeof(T));
            }
    };
}
