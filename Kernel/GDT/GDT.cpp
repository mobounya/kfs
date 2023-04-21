#include <Kernel/GDT/GDT.hpp>

namespace Kernel
{
    GDT_Table        *GDT::gdt_ptr = NULL;
    GDT_Descriptor   *GDT::gdt_descriptor_ptr = NULL;

    GDT_Segment_Descriptor           *GDT_Table::insert_new_entry(const GDT_Segment_Descriptor &entry, size_t index)
    {
        if (index >= 8192)
            return NULL;
        this->entries[index] = entry;
        return &(this->entries[index]);
    }

    const GDT_Segment_Descriptor     *GDT_Table::get_entry(size_t index) const
    {
        if (index >= 8192)
            return NULL;
        return &(this->entries[index]);
    }

    void            GDT_Segment_Descriptor::set_limit(uint32_t limit)
    {
        this->limit_1 = limit & 0xffff;
        this->limit_2 = (limit >> 16) & 0xf;
    }

    uint32_t        GDT_Segment_Descriptor::get_limit(void) const
    {
        uint32_t limit = this->limit_1;
        limit |= (uint32_t)(limit_2 << 16);
        return limit;
    }

    void        GDT_Segment_Descriptor::set_base(uint32_t base)
    {
        this->base_1 = base & 0xffffff;
        this->base_2 = (base >> 24) & 0xff;
    }

    uint32_t    GDT_Segment_Descriptor::get_base(void) const
    {
        uint32_t base = this->base_1;
        base |= (uint32_t)this->base_2 << 24;
        return base;
    }

    void        GDT_Segment_Descriptor::set_access_byte(uint8_t access_byte)
    {
        this->access_byte = access_byte;
    }

    uint8_t     GDT_Segment_Descriptor::get_access_byte(void) const
    {
        return this->access_byte;
    }

    void        GDT_Segment_Descriptor::set_flags(uint8_t flags)
    {
        this->flags = flags;
    }

    uint8_t     GDT_Segment_Descriptor::get_flags(void) const
    {
        return this->flags;
    }

    void        GDT_Descriptor::set_size(uint16_t size)
    {
        this->size = size;
    }

    uint16_t    GDT_Descriptor::get_size(void) const
    {
        return this->size;
    }

    void        GDT_Descriptor::set_offset(uint32_t offset)
    {
        this->offset = offset;
    }

    uint32_t        GDT_Descriptor::get_offset(void) const
    {
        return this->offset;
    }

    GDT_Segment_Descriptor GDT::create_GDT_Descriptor(uint32_t base, uint32_t limit, uint8_t access_byte, uint8_t flags)
    {
        GDT_Segment_Descriptor descriptor;
        descriptor.set_limit(limit);
        descriptor.set_base(base);
        descriptor.set_access_byte(access_byte);
        descriptor.set_flags(flags);
        return descriptor;
    }

    const GDT_Table        *GDT::get_gdt_ptr(void)
    {
        return gdt_ptr;
    }

    const GDT_Descriptor     *GDT::get_gdt_descriptor_ptr(void)
    {
        return gdt_descriptor_ptr;
    }
}
