#pragma once

#include <stdint.h>

#define CONTAINER_SIZE 100

namespace std
{
    template <typename T>
    class vector
    {
        typedef uint32_t    size_type;
        typedef const T&    const_reference;

        private:
            // NOTE: Don't judge me we don't have memory allocation.
            T container[CONTAINER_SIZE];
            size_type m_size;
        public:
            vector()
            {
                m_size = 0;
            }

            const_reference    operator[](size_type pos) const
            {
                return container[pos];
            }

            T*          at(size_type pos)
            {
                if (pos < 0 || pos >= m_size)
                    return NULL;
                else
                    return container + pos;
            }

            T*          front()
            {
                if (m_size > 0)
                    return container;
                else
                    return NULL;
            }

            T*          back()
            {
                if (m_size > 0)
                    return container + (m_size - 1);
                else
                    return (NULL);
            }

            size_type    capacity() const
            {
                return CONTAINER_SIZE;
            }

            bool        empty() const
            {
                return (m_size == 0);
            }

            size_type    size() const
            {
                return m_size;
            }

            T*          insert(size_type index, const T& value)
            {
                if (index < 0 || index >= m_size)
                    return NULL;
                container[index] = value;
                return (container + index);
            }

            T*          erase(size_type pos)
            {
                if (pos > m_size)
                    return NULL;
                for (size_type i = (pos + 1); i < m_size; i++)
                    container[i - 1] = container[i];
                m_size--;
                return (container + pos);
            }

            void        push_back(const T& value)
            {
                if (m_size < capacity())
                {
                    container[m_size] = value;
                    m_size++;
                }
            }

            void        pop_back()
            {
                if (m_size > 0)
                    m_size--;
            }
    };
}
