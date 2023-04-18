#pragma once

#include <stdint.h>
#include <stddef.h>

#include <Kernel/CPU/Registers.hpp>

class CPU
{
    private:
        Registers       m_regs;
        static CPU      m_cpu;
        static bool     instantiated;

    private:
        CPU();

    public:
        static CPU          &instantiate(void);
        static void         outb(uint16_t port, uint8_t val);
        static uint8_t      inb(uint16_t port);
        static void         io_wait(void);
        static void         panic(void);

    public:
        Registers           save_state(void);
        uint32_t            get_eax(void);
        uint32_t            get_ebx(void);
        uint32_t            get_ecx(void);
        uint32_t            get_edx(void);
        uint32_t            get_esp(void);
        uint32_t            get_ebp(void);
        uint32_t            get_esi(void);
        uint32_t            get_edi(void);
};
