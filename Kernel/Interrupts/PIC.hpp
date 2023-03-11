#pragma once

#include <stdint.h>

#define MASTER_PIC_COMMAND_PORT 0x20
#define MASTER_PIC_DATA_PORT 0x21

#define SLAVE_PIC_COMMAND_PORT 0xA0
#define SLAVE_PIC_DATA_PORT 0xA1

#define ICW1_ICW4	    0x01	/* ICW4 (not) needed */
#define ICW1_SINGLE	    0x02	/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04    /* Call address interval 4 (8) */
#define ICW1_LEVEL	    0x08	/* Level triggered (edge) mode */
#define ICW1_INIT	    0x10	/* Initialization - required! */
 
#define ICW4_8086	    0x01	/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	    0x02	/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08	/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C	/* Buffered mode/master */
#define ICW4_SFNM	    0x10	/* Special fully nested (not) */


namespace Interrupts
{
    class PIC8259
    {
        public:
            static void PIC_remap(int slave_offset, int master_offset);
    };

    typedef PIC8259 PIC;
}
