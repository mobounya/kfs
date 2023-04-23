#pragma once

#include <stdint.h>
#include <stddef.h>

#include <Kernel/Memory/Address.hpp>

class VirtualAddress : public Address
{
    public:
        VirtualAddress();
        VirtualAddress(void *ptr);
        VirtualAddress(const void *ptr);
        VirtualAddress(uint32_t ptr);
};
