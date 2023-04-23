#pragma once

#include <stdint.h>
#include <stddef.h>

#include <Kernel/Memory/Address.hpp>

class PhysicalAddress : public Address
{
    public:
        PhysicalAddress();
        PhysicalAddress(void *ptr);
        PhysicalAddress(const void *ptr);
        PhysicalAddress(uint32_t ptr);

    public:
        PhysicalAddress operator<<(size_t bits);
        PhysicalAddress operator>>(size_t bits);
};
