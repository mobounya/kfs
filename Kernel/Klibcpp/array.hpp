#pragma once

#include <stdint.h>
#include <stddef.h>

namespace std
{
    // TODO: implement Iterators.
    template <typename T, size_t N>
    class array
    {
        typedef T                   value_type;
        typedef size_t              size_type;
        typedef value_type&         reference;
        typedef const value_type&   const_reference;
        typedef T*                  pointer;
        typedef const T*            const_pointer;

        private:
            value_type data[N];
            size_t m_size = N;

        public:
            reference at(size_type pos)
            {
                return data[pos];
            }

            const_reference at(size_type pos) const
            {
                return data[pos];
            }

            reference operator[](size_type pos)
            {
                return at(pos);
            }

            const_reference operator[](size_type pos) const
            {
                return at(pos);
            }

            reference front()
            {
                return data[0];
            }

            const_reference front() const
            {
                return data[0];
            }

            reference back()
            {
                return data[m_size - 1];
            }

            const_reference back() const
            {
                return data[m_size - 1];
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

            void fill(const T& value)
            {
                for (size_type i = 0; i < N; i++)
                    data[i] = value;
            }

            void swap(array& other)
            {
                for (size_type i = 0; i < N && i < other.size(); i++)
                {
                    value_type temp = data[i];
                    data[i] = other[i];
                    other[i] = temp;
                }
            }

            bool operator==(const array<T, N> &v) const
            {
                for (size_type i = 0; i < m_size && i < v.size(); i++)
                {
                    if (data[i] != v[i])
                        return false;
                }
                return true;
            }

            bool operator!=(const array<T, N> &v) const
            {
                return !(*this == v);
            }

            bool operator<(const array<T, N> &v) const
            {
                size_type i = 0;
                while (i < size() && data[i] == v[i])
                    i++;
                return (i != size() && data[i] < v[i]);
            }

            bool operator<=(const array<T, N> &v) const
            {
                return (*this == v || *this < v);
            }
    
            bool operator>(const array<T, N> &v) const
            {
                size_type i = 0;
                while (i < size() && data[i] == v[i])
                    i++;
                return (i != size() && data[i] > v[i]);
            }

            bool operator>=(const array<T, N> &v) const
            {
                return (*this == v || *this > v);
            }
    };
}