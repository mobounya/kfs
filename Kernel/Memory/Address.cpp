#include <Kernel/Memory/Address.hpp>

Address::Address()
{
    this->m_ptr = NULL;
}

Address::Address(void *ptr)
{
    this->m_ptr = ptr;
}

Address::Address(const void *ptr)
{
    this->m_ptr = (void *)ptr;
}

Address::Address(uint32_t ptr)
{
    this->m_ptr = (void *)ptr;
}

void    *Address::ptr(void) const
{
    return m_ptr;
}

Address &Address::operator=(void *ptr)
{
    this->m_ptr = ptr;
    return *this;
}

Address &Address::operator=(const Address &addr)
{
    this->m_ptr = addr.ptr();
    return *this;
}

Address &Address::operator+=(size_t size)
{
    this->m_ptr = (void *)((uint8_t *)(this->m_ptr) + size);
    return *this;
}

Address Address::operator+(size_t size)
{
    void *tmp = (void *)(((uint8_t *)this->m_ptr) + size);
    return Address(tmp);
}

bool    Address::operator>(const Address &addr) const
{
    return ((uint32_t)(this->m_ptr) > (uint32_t)(addr.ptr()));
}

bool    Address::operator>(const void *ptr) const
{
    return ((uint32_t)(this->m_ptr) > (uint32_t)(ptr));
}

bool    Address::operator<(const Address &addr) const
{
    return ((uint32_t)(this->m_ptr) < (uint32_t)(addr.ptr()));
}

bool    Address::operator<(const void *ptr) const
{
    return ((uint32_t)(this->m_ptr) < (uint32_t)(ptr));
}

bool        Address::operator==(const Address &addr) const
{
    return (this->m_ptr == addr.ptr());
}

bool        Address::operator==(const void *ptr) const
{
    return (this->m_ptr == ptr);
}
