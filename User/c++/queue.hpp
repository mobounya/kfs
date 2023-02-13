#pragma once

#include <stdint.h>

#define CONTAINER_SIZE 100

namespace std
{
    template <typename T>
    class queue
    {
        typedef uint32_t    size_type;
        typedef const T&    const_reference;

        private:
            // NOTE: Don't judge me we don't have memory allocation.
            T container[CONTAINER_SIZE];
            size_type m_size;
        public:
            queue()
            {
                m_size = 0;
            }

            bool empty(void) const
            {
                return (m_size == 0);
            }

            size_type size(void) const
            {
                return m_size;
            }

            void pop(void)
            {
                if (m_size > 0)
                {
                    for (int i = 1; i < m_size; i++)
                        container[i - 1] = container[i];
                    m_size--;
                }
            }

            T *front(void)
            {
                if (m_size > 0)
                    return container;
                else
                    return NULL;
            }

            void push(const_reference value)
            {
                if (m_size < CONTAINER_SIZE)
                {
                    container[m_size] = value;
                    m_size++;
                }
            }

            T*  back(void)
            {
                if (m_size > 0)
                    return container + (m_size - 1);
                else
                    return NULL; 
            }
    };
}
