#pragma once

#include <stdint.h>
#include <stddef.h>

#include <Kernel/Interrupts/IDT.hpp>

#define SIGNAL_1 0
#define SIGNAL_2 1
#define SIGNAL_3 2

class Signals
{
    typedef void (*singal_handler)(void);

    private:
        Interrupts::InterruptDescriptorTable    *interrupt_descriptor_table;

    public:
        Signals();
        Signals(Interrupts::InterruptDescriptorTable *interrupt_descriptor_table);

    public:
        void            register_signal(int sig, singal_handler fn);
        static void     send_signal(int sig);
};
