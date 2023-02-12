#pragma once

#include <stdint.h>

#define CONTAINER_SIZE 100

namespace std
{
    template <typename T>
    class queue
    {
        private:
            // NOTE: Don't judge me we don't have memory allocation.
            T container[CONTAINER_SIZE];
            uint16_t index;
        public:
            queue()
            {
                index = 0;
            }

            bool empty(void) const
            {
                return (index == 0);
            }

            uint16_t size(void) const
            {
                return index;
            }

            void pop(void)
            {
                if (index > 0)
                {
                    for (int i = 1; i < index; i++)
                        container[i - 1] = container[i];
                    index--;
                }
            }

            T& front(void)
            {
                return container[0];
            }

            void push(const T& value)
            {
                container[index] = value;
                index++;
            }

            T& back(void)
            {
                return container[index - 1];
            }
    };
}
