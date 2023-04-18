#include <Kernel/CPU/CPU.hpp>

CPU      CPU::m_cpu;
bool     CPU::instantiated = false;

CPU::CPU()
{
}

CPU     &CPU::instantiate(void)
{
    if (instantiated == false)
    {
        m_cpu = CPU();
        instantiated = true;
    }
    return m_cpu;
}

void CPU::outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
        * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
        * The  outb  %al, %dx  encoding is the only option for all other cases.
        * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

uint8_t CPU::inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                    : "=a"(ret)
                    : "Nd"(port) );
    return ret;
}

void CPU::io_wait(void)
{
    outb(0x80, 0);
}

uint32_t    CPU::get_eax(void)
{
    asm( 
        " mov %%eax, %0\n"
        :  "=r" (m_regs.eax)
    );

    return m_regs.eax;
}

uint32_t    CPU::get_ebx(void)
{
    asm( 
        " mov %%ebx, %0\n"
        :  "=r" (m_regs.ebx) /* %0: Output variable list */
    );

    return m_regs.ebx;
}

uint32_t    CPU::get_ecx(void)
{
    asm( 
        " mov %%ecx, %0\n"
        :  "=r" (m_regs.ecx) /* %0: Output variable list */
    );

    return m_regs.ecx;
}

uint32_t    CPU::get_edx(void)
{
    asm( 
        " mov %%edx, %0\n"
        :  "=r" (m_regs.edx) /* %0: Output variable list */
    );

    return m_regs.edx;
}

uint32_t    CPU::get_esp(void)
{
    asm( 
        " mov %%esp, %0\n"
        :  "=r" (m_regs.esp) /* %0: Output variable list */
    );

    return m_regs.esp;
}

uint32_t    CPU::get_ebp(void)
{
    asm( 
        " mov %%ebp, %0\n"
        :  "=r" (m_regs.ebp) /* %0: Output variable list */
    );

    return m_regs.ebp;
}

uint32_t    CPU::get_esi(void)
{
    asm( 
        " mov %%esi, %0\n"
        :  "=r" (m_regs.esi) /* %0: Output variable list */
    );

    return m_regs.esi;
}

uint32_t    CPU::get_edi(void)
{
    asm(
        " mov %%edi, %0\n"
        :  "=r" (m_regs.edi) /* %0: Output variable list */
    );

    return m_regs.edi;
}

Registers   CPU::save_state(void)
{
    get_eax();
    get_ebx();
    get_ecx();
    get_edx();
    get_esp();
    get_ebp();
    get_esi();
    get_edi();
    return m_regs;
}
