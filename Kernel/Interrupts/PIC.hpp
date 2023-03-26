#pragma once

#include <stdint.h>

#define PIC1		    0x20		/* IO base address for master PIC */
#define PIC2	    	0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)

#define ICW1_ICW4	    0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	    0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	    0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	    0x10		/* Initialization - required! */
 
#define ICW4_8086	    0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	    0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	    0x10		/* Special fully nested (not) */

#define PIC_EOI		    0x20		/* End-of-interrupt command code */

namespace Interrupts
{
    class PIC8259
    {
        public:
            static void     PIC_remap(int offset1, int offset2);
            static void     PIC_sendEOI(unsigned char irq);
            static void     IRQ_set_mask(unsigned char IRQline);
            static void     IRQ_clear_mask(unsigned char IRQline);
            static void     mask_all(void);
            static void     enable(void);
            static void     disable(void);
    };

    typedef PIC8259 PIC;
}
