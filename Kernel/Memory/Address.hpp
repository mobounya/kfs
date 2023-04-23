#pragma once

#include <stdint.h>
#include <stddef.h>

class Address
{
    protected:
        void *m_ptr;

    public:
        Address();
        Address(void *ptr);
        Address(const void *ptr);
        Address(uint32_t ptr);

    public:
        Address     &operator=(void *ptr);
        Address     &operator=(const Address &addr);
        Address     &operator+=(size_t size);
        Address     operator+(size_t size);
        bool        operator>(const Address &addr) const;
        bool        operator>(const void *ptr) const;
        bool        operator<(const Address &addr) const;
        bool        operator<(const void *ptr) const;
        bool        operator==(const Address &addr) const;
        bool        operator==(const void *ptr) const;

    public:
        void    *ptr(void) const;
};
