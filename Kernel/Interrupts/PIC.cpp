#include <Kernel/Interrupts/PIC.hpp>
#include <Kernel/CPU/CPU.hpp>

namespace Interrupts
{
	/*
	arguments:
		offset1 - vector offset for master PIC
			vectors on the master become offset1..offset1+7
		offset2 - same for slave PIC: offset2..offset2+7
	*/
	void PIC::PIC_remap(int offset1, int offset2)
	{
		unsigned char a1, a2;
	
		a1 = CPU::inb(PIC1_DATA);                        // save masks
		a2 = CPU::inb(PIC2_DATA);
	
		CPU::outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
		CPU::io_wait();
		CPU::outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
		CPU::io_wait();
		CPU::outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
		CPU::io_wait();
		CPU::outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
		CPU::io_wait();
		CPU::outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
		CPU::io_wait();
		CPU::outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
		CPU::io_wait();
	
		CPU::outb(PIC1_DATA, ICW4_8086);
		CPU::io_wait();
		CPU::outb(PIC2_DATA, ICW4_8086);
		CPU::io_wait();
	
		CPU::outb(PIC1_DATA, a1);   // restore saved masks.
		CPU::outb(PIC2_DATA, a2);
	}

	void PIC::PIC_sendEOI(unsigned char irq)
	{
		if(irq >= 8)
			CPU::outb(PIC2_COMMAND,PIC_EOI);
	
		CPU::outb(PIC1_COMMAND,PIC_EOI);
	}

	void PIC::IRQ_set_mask(unsigned char IRQline)
	{
		uint16_t port;
		uint8_t value;
	
		if (IRQline < 8) {
			port = PIC1_DATA;
		} else {
			port = PIC2_DATA;
			IRQline -= 8;
		}
		value = CPU::inb(port) | (1 << IRQline);
		CPU::outb(port, value);        
	}
	
	void PIC::IRQ_clear_mask(unsigned char IRQline)
	{
		uint16_t port;
		uint8_t value;
	
		if (IRQline < 8) {
			port = PIC1_DATA;
		} else {
			port = PIC2_DATA;
			IRQline -= 8;
		}
		value = CPU::inb(port) & ~(1 << IRQline);
		CPU::outb(port, value);
	}

	void	PIC::mask_all(void)
	{
		CPU::outb(PIC1_DATA, 0xff);
   		CPU::outb(PIC2_DATA, 0xff);
	}

	void     PIC::enable(void)
	{
		asm("sti");
	}

	void     PIC::disable(void)
	{
		asm("cli");
	}
}
