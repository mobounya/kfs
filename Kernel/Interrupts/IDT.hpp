#pragma once

#include <stdint.h>
#include <stddef.h>

#define IDT_SIZE 256

#define IRQ_START 0x20
#define IRQ_SIZE 16

#define SOFTWARE_INTERRUPTS_START (IRQ_START + IRQ_SIZE)
#define SOFTWARE_INTERRUPTS_SIZE (224 - IRQ_SIZE)

namespace Interrupts
{
    enum GateType
    {
        Undefined = 0x0,
        TaskGate = 0x5,
        InterruptGate16 = 0x6,
        TrapGate16 = 0x7,
        InterruptGate32 = 0xE,
        TrapGate32 = 0xF
    };

    struct GateDescriptor32
    {
        private:
            uint32_t    offset_1 : 16;
            uint32_t    segment_selector : 16;
            uint32_t    reserved : 8;
            uint32_t    gate_type : 4;
            uint32_t    zero : 1;
            uint32_t    DPL : 2;
            uint32_t    present : 1;
            uint32_t    offset_2 : 16;

        public:
            void                        set_offset(uint32_t offset);
            uint32_t                    get_offset(void) const;
            void                        set_segment_selector(uint16_t segment_selector);
            uint16_t                    get_segment_selector(void) const;
            void                        set_gate_type(GateType gate_type);
            GateType                    get_gate_type(void) const;
            void                        set_DPL(uint8_t DPL);
            uint8_t                     get_DPL(void) const;
            void                        set_present(bool present);
            bool                        is_present(void) const;
            static GateDescriptor32     create_gate_descriptor(uint32_t offset, uint16_t segment_selector, GateType gate_type, uint8_t DPL, bool present);
    } __attribute__((packed));

    struct InterruptDescriptorTable32
    {
        private:
            GateDescriptor32    entries[IDT_SIZE];

        public:
            InterruptDescriptorTable32(void);

        public:
            void                    insert_intel_interrupt_handler(const GateDescriptor32 &entry, size_t index);
            void                    insert_irq_handler(const GateDescriptor32 &entry, size_t offset);
            void                    insert_software_interrupt_handler(const GateDescriptor32 &entry, size_t offset);
            const GateDescriptor32  *get_entry(size_t index) const;
    } __attribute__((packed));

    struct IDTDescriptor
    {
        private:
            uint16_t size;
            uint32_t address;
        public:
            void        set_size(uint16_t size);
            uint16_t    get_size(void) const;
            void        set_address(uint32_t address);
            uint32_t    get_address(void) const;
    } __attribute__((packed));

    typedef InterruptDescriptorTable32  InterruptDescriptorTable;
    typedef GateDescriptor32            GateDescriptor;
}
