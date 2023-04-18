#include <Kernel/Debug/Debug.hpp>
#include <Kernel/CPU/CPU.hpp>
#include <Kernel/Display/Screen.hpp>

void    Debug::dump_registers(void)
{
    Screen      cout;
    CPU         cpu = CPU::instantiate();
    Registers   regs = cpu.save_state();

    cout << "registers dump:" << "\n";
    cout << "%eax: " << regs.eax << "   %ebx: " << regs.ebx << "   %ecx: " << regs.ecx << "\n";
    cout << "%edx: " << regs.edx << "   %esp: " << regs.esp << "   %ebp: " << regs.ebp << "\n";
    cout << "%esi: " << regs.esi << "   %edi: " << regs.edi << "\n";
    return ;
}

void    Debug::dump_stack(void)
{
    return ;
}
