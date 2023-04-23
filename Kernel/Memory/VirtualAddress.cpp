#include <Kernel/Memory/VirtualAddress.hpp>

VirtualAddress::VirtualAddress()
{
    
}

VirtualAddress::VirtualAddress(void *ptr) : Address(ptr)
{

}

VirtualAddress::VirtualAddress(const void *ptr) : Address(ptr)
{

}

VirtualAddress::VirtualAddress(uint32_t ptr) : Address(ptr)
{

}
