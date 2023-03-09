#include <Kernel/Interrupts/PIC.hpp>

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
        * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
        * The  outb  %al, %dx  encoding is the only option for all other cases.
        * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                    : "=a"(ret)
                    : "Nd"(port) );
    return ret;
}

static inline void io_wait(void)
{
    outb(0x80, 0);
}

namespace Interrupts
{
    void PIC::PIC_remap(int slave_offset, int master_offset)
    {
	    unsigned char a1, a2;
    
	    a1 = inb(MASTER_PIC_DATA_PORT);                        // save masks
	    a2 = inb(SLAVE_PIC_DATA_PORT);
    
	    outb(MASTER_PIC_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	    io_wait();
	    outb(SLAVE_PIC_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);
	    io_wait();
	    outb(MASTER_PIC_DATA_PORT, master_offset);                 // ICW2: Master PIC vector offset
	    io_wait();
	    outb(SLAVE_PIC_DATA_PORT, slave_offset);                 // ICW2: Slave PIC vector offset
	    io_wait();
	    outb(MASTER_PIC_DATA_PORT, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	    io_wait();
	    outb(SLAVE_PIC_DATA_PORT, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	    io_wait();
    
	    outb(MASTER_PIC_DATA_PORT, ICW4_8086);
	    io_wait();
	    outb(SLAVE_PIC_DATA_PORT, ICW4_8086);
	    io_wait();
    
	    outb(MASTER_PIC_DATA_PORT, a1);   // restore saved masks.
	    outb(SLAVE_PIC_DATA_PORT, a2);
        return ;
    }
}
