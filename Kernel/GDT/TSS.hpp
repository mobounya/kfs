#pragma once

#include <stddef.h>
#include <stdint.h>


#define TSS_SIZE 27
#define TSS_SIZE_BYTES (TSS_SIZE * 4)

#define ESP0_INDEX  0x1
#define SS0_INDEX   0x2
#define IOPB_INDEX  0x19

// https://wiki.osdev.org/Task_State_Segment
namespace Kernel
{
    struct TSS
    {
        private:
            uint32_t     fields[TSS_SIZE];
        public:
            void            set_SS0(uint16_t ss0);
            uint16_t        get_SS0(void) const;
            void            set_ESP0(uint32_t esp0);
            uint32_t        get_ESP0(void) const;
            void            set_IOPB(uint16_t iopb);
            uint16_t        get_IOPB(void) const;
    };
}
