#pragma once

#include <stdint.h>
#include <cstring.h>

#include <Kernel/Memory/QuickDirtyMalloc.hpp>

namespace std
{
    class string
    {
        typedef char                         value_type;
        typedef size_t                       size_type;
        typedef value_type&                  reference;
        typedef const value_type&            const_reference;
        typedef value_type*                  pointer;
        typedef const value_type*            const_pointer;

        private:
            static const size_type npos = -1;
            pointer m_data;
            size_t m_size;
            size_t m_capacity;

        public:
            string()
            {
                m_data = NULL;
                m_size = 0;
            }

            string(const_pointer str)
            {
                size_type i = 0;
                size_type len = strlen(str);
                m_size = len;
                m_capacity = get_actual_allocated_size(len + 1);

                m_data = (pointer)quick_dirty_kmalloc(len + 1);
                for (; i < len; i++)
                    m_data[i] = str[i];

                m_data[i] = '\0';
            }
    
            string(size_type count, value_type ch)
            {
                size_type i = 0;
                m_size = count;
                m_capacity = get_actual_allocated_size(count);

                m_data = (pointer)quick_dirty_kmalloc(count + 1);
                for (; i < count; i++)
                    m_data[i] = ch;

                m_data[i] = '\0';
            }

            string(const string& str)
            {
                *this = str;
            }
        
            ~string()
            {
                quick_dirty_kfree(m_data);
            }

        public:
            string& operator=(const string& str)
            {
                m_size = str.m_size;
                m_capacity = str.m_capacity;
                m_data = (pointer)quick_dirty_kmalloc(str.m_capacity + 1);
                memcpy(m_data, str.m_data, str.m_size);
                return *this;
            }

            bool operator==(const string& str)
            {
                return equal(str);
            }

        public:
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
                return m_data[m_size - 1];
            }

            const_reference back() const
            {
                return m_data[m_size - 1];
            }

            const_pointer data() const
            {
                return m_data;
            }

            pointer data()
            {
                return m_data;
            }

            const_pointer c_str() const
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

            size_type length() const
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
                pointer temp = (pointer)quick_dirty_kmalloc(new_cap + 1);
                m_capacity = get_actual_allocated_size(new_cap + 1);
                size_type i = 0;
                for (; i < m_size; i++)
                    temp[i] = m_data[i];
                temp[i] = '\0';
                quick_dirty_kfree(m_data);
                m_data = temp;
            }

            size_type capacity() const
            {
                return m_capacity;
            }

            void clear()
            {
                for (size_type i = 0; i < m_size; i++)
                    m_data[i] = '\0';
                m_size = 0;
            }

            string& insert(size_type index, size_type count, value_type ch)
            {
                if (count == 0)
                    return *this;
                char *str1 = NULL;
                size_type new_string_size = count;
                if (index > 0)
                {
                    str1 = strndup(m_data, index);
                    new_string_size += strlen(str1);
                }
                char *str2 = strndup(m_data + index, m_size);
                new_string_size += strlen(str2);
                char *tmp = (pointer)quick_dirty_kmalloc(new_string_size + 1);
                if (index > 0)
                    strcat(tmp, str1);
                for (size_type i = 0; i < count; i++)
                    tmp[index + i] = ch;
                strcat(tmp + index + count, str2);
                tmp[new_string_size] = '\0';
                quick_dirty_kfree(m_data);
                m_data = tmp;
                m_size = new_string_size;
                m_capacity = get_actual_allocated_size(new_string_size + 1);
                return *this;
            }

            string& insert(size_type index, const_pointer s)
            {
                char *str1 = NULL;
                size_type str1_length = 0;
                size_type str2_length = 0;
                size_type new_string_size = 0;
                size_type s_length;
                if (s == NULL || (s_length = strlen(s)) == 0)
                    return *this;
                new_string_size += s_length;
                if (index > 0)
                {
                    str1 = strndup(m_data, index);
                    str1_length = strlen(str1);
                    new_string_size += str1_length;
                }
                char *str2 = strndup(m_data + index, m_size);
                str2_length = strlen(str2);
                new_string_size += str2_length;
                
                char *tmp = (pointer)quick_dirty_kmalloc(new_string_size + 1);
                if (index > 0)
                    strcpy(tmp, str1);
                strcpy(tmp + str1_length, s);
                strcpy(tmp + str1_length + s_length, str2);
                tmp[new_string_size] = '\0';
                quick_dirty_kfree(m_data);
                m_data = tmp;
                m_size = new_string_size;
                m_capacity = get_actual_allocated_size(new_string_size + 1);
                return *this;
            }

            void push_back(value_type ch)
            {
                if (m_capacity < m_size + 1)
                    reserve(m_size + 1);
                m_data[m_size] = ch;
                m_data[m_size + 1] = '\0';
                m_size++;
            }

            void pop_back(void)
            {
                if (m_size > 0)
                {
                    m_data[m_size - 1] = '\0';
                    m_size--;
                }
            }

            string& append(size_type count, value_type ch)
            {
                size_type i = 0;
                if (count == 0)
                    return *this;
                if (m_capacity < m_size + count)
                    reserve(m_size + count);
                for (; i < count; i++)
                    m_data[m_size + i] = ch;
                m_data[m_size + i] = '\0';
                m_size += count;
                return *this;
            }

            string& operator+=(value_type ch)
            {
                return append(1, ch);
            }

            string& operator+=(const_pointer s)
            {
                for (size_type i = 0; i < strlen(s); i++)
                    append(1, s[i]);
                return *this;
            }

            int compare(const string& str) const
            {
                size_type str_size = str.size();
                if (m_size > str_size)
                    return 1;
                if (str_size > m_size)
                    return -1;
                size_type i = 0;
                while (i < size() && i < str.size() && m_data[i] == str[i])
                    i++;
                return m_data[i] - str[i];
            }

            bool equal(const string& str) const
            {
                return (compare(str) == 0);
            }

            bool contains(value_type ch) const
            {
                for (size_type i = 0; i < m_size; i++)
                {
                    if (m_data[i] == ch)
                        return true;
                }
                return false;
            }

            bool contains(const_pointer str) const
            {
                for (size_type i = 0; i < m_size; i++)
                {
                    if (m_data[i] == str[0])
                    {
                        size_type str_length = strlen(str);
                        size_type temp_i = i;
                        size_type j = 0;
                        while (temp_i < m_size && j < str_length && m_data[temp_i] == str[j])
                        {
                            temp_i++;
                            j++;
                        }
                        if (j == str_length)
                            return true;
                    }
                }
                return false;
            }

            string substr(size_type pos, size_type count) const
            {
                size_type size_to_allocate;
                if (count == 0)
                    return string();
                if ((m_size - pos) < count)
                    size_to_allocate = (m_size - pos);
                else
                    size_to_allocate = count;
                char *temp = (pointer)quick_dirty_kmalloc(size_to_allocate + 1);
                size_type i = 0;
                size_type j = pos;
                for (; i < size_to_allocate; i++, j++)
                    temp[i] = m_data[j];
                temp[i] = '\0';
                string string_temp(temp);
                quick_dirty_kfree(temp);
                return string_temp;
            }
    };
}
