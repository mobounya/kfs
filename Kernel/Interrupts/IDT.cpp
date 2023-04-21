#include <Kernel/Interrupts/IDT.hpp>

namespace Interrupts
{
    void                    GateDescriptor32::set_offset(uint32_t offset)
    {
        uint16_t offset_1 = offset & 0xffff;
        uint16_t offset_2 = (offset >> 16) & 0xffff;
        this->offset_1 = offset_1;
        this->offset_2 = offset_2;
    }

    uint32_t                GateDescriptor32::get_offset(void) const
    {
        uint32_t offset = (uint32_t)this->offset_1;
        offset = (((uint32_t)this->offset_2) << 16) | offset;
        return offset;
    }

    // https://wiki.osdev.org/Segment_Selector
    void                    GateDescriptor32::set_segment_selector(uint16_t segment_selector)
    {
        this->segment_selector = segment_selector;
    }

    uint16_t                GateDescriptor32::get_segment_selector(void) const
    {
        return this->segment_selector;
    }

    void                    GateDescriptor32::set_gate_type(GateType gate_type)
    {
        this->gate_type = gate_type;
    }

    GateType                GateDescriptor32::get_gate_type(void) const
    {
        if (this->gate_type == GateType::TaskGate)
            return GateType::TaskGate;
        if (this->gate_type == GateType::InterruptGate16)
            return GateType::InterruptGate16;
        if (this->gate_type == GateType::TrapGate16)
            return GateType::TrapGate16;
        if (this->gate_type == GateType::InterruptGate32)
            return GateType::InterruptGate32;
        if (this->gate_type == GateType::TrapGate32)
            return GateType::TrapGate32;
        return GateType::Undefined;
    }

    GateDescriptor32        GateDescriptor32::create_gate_descriptor(uint32_t offset, uint16_t segment_selector, GateType gate_type, uint8_t DPL, bool present)
    {
        GateDescriptor32 descriptor;
        descriptor.set_offset(offset);
        descriptor.set_segment_selector(segment_selector);
        descriptor.set_gate_type(gate_type);
        descriptor.set_DPL(DPL);
        descriptor.set_present(present);
        return descriptor;
    }

    void                    GateDescriptor32::set_DPL(uint8_t DPL)
    {
        this->DPL = DPL;
    }

    uint8_t                 GateDescriptor32::get_DPL(void) const
    {
        return this->DPL;
    }

    void                    GateDescriptor32::set_present(bool present)
    {
        this->present = present;
    }

    bool                    GateDescriptor32::is_present(void) const
    {
        return this->present;
    }

    InterruptDescriptorTable32::InterruptDescriptorTable32()
    {
    }

    void                    InterruptDescriptorTable32::insert_intel_interrupt_handler(const GateDescriptor32 &entry, size_t index)
    {
        if (index <= 20)
            this->entries[index] = entry;
    }

    void                    InterruptDescriptorTable32::insert_irq_handler(const GateDescriptor32 &entry, size_t offset)
    {
        if (offset < IRQ_SIZE)
            this->entries[IRQ_START + offset] = entry;
    }

    void                    InterruptDescriptorTable32::insert_software_interrupt_handler(const GateDescriptor32 &entry, size_t offset)
    {
        if (offset < SOFTWARE_INTERRUPTS_SIZE)
            this->entries[SOFTWARE_INTERRUPTS_START + offset] = entry;
    }

    const GateDescriptor32  *InterruptDescriptorTable32::get_entry(size_t index) const
    {
        if (index < IDT_SIZE)
            return &(this->entries[index]);
        else
            return NULL;
    }

    void        IDTDescriptor::set_size(uint16_t size)
    {
        this->size = size;
    }

    uint16_t    IDTDescriptor::get_size(void) const
    {
        return this->size;
    }

    void        IDTDescriptor::set_address(uint32_t address)
    {
        this->address = address;
    }

    uint32_t    IDTDescriptor::get_address(void) const
    {
        return this->address;
    }
}
