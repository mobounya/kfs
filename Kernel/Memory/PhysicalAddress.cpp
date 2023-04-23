#include <Kernel/Memory/PhysicalAddress.hpp>

PhysicalAddress::PhysicalAddress()
{

}

PhysicalAddress::PhysicalAddress(void *ptr) : Address(ptr)
{

}

PhysicalAddress::PhysicalAddress(const void *ptr) : Address(ptr)
{

}

PhysicalAddress::PhysicalAddress(uint32_t ptr) : Address(ptr)
{

}

PhysicalAddress PhysicalAddress::operator<<(size_t bits)
{
    uint32_t temp = (uint32_t)(this->m_ptr) << bits;
    return PhysicalAddress(temp);
}

PhysicalAddress PhysicalAddress::operator>>(size_t bits)
{
    uint32_t temp = (uint32_t)(this->m_ptr) >> bits;
    return PhysicalAddress(temp);
}
