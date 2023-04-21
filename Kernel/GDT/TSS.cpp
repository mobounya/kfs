#include <Kernel/GDT/TSS.hpp>

namespace Kernel
{
    void        TSS::set_SS0(uint16_t ss0)
    {
        this->fields[SS0_INDEX] = ss0;
    }

    uint16_t    TSS::get_SS0(void) const
    {
        return this->fields[SS0_INDEX];
    }

    void        TSS::set_ESP0(uint32_t esp0)
    {
        this->fields[ESP0_INDEX] = esp0;
    }

    uint32_t    TSS::get_ESP0(void) const
    {
        return this->fields[ESP0_INDEX];
    }

    void        TSS::set_IOPB(uint16_t iopb)
    {
        this->fields[IOPB_INDEX] = (uint32_t)iopb << 16;
    }

    uint16_t    TSS::get_IOPB(void) const
    {
        return (this->fields[IOPB_INDEX] >> 16);
    }
}