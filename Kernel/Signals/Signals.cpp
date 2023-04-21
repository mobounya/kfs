#include <Kernel/Signals/Signals.hpp>

Signals::Signals(void)
{
    this->interrupt_descriptor_table = NULL;
}

Signals::Signals(Interrupts::InterruptDescriptorTable *interrupt_descriptor_table)
{
    this->interrupt_descriptor_table = interrupt_descriptor_table;
}

/*
    Note that interrupt handlers are not just like any normal function.
    Please read: https://wiki.osdev.org/Interrupt_Service_Routines
*/

void    Signals::register_signal(int sig, singal_handler handler)
{
    size_t int_vector;

    if (sig == SIGNAL_1 || sig == SIGNAL_2 || sig == SIGNAL_3)
        int_vector = sig;
    else
        return ;

    Interrupts::GateDescriptor descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)handler, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_software_interrupt_handler(descriptor, int_vector);
}

void    Signals::send_signal(int sig)
{
    if (sig == SIGNAL_1)
        asm("int $48");
    else if (sig == SIGNAL_2)
        asm("int $49");
    else if (sig == SIGNAL_3)
        asm("int $50");
}
