#include <Kernel/Debug/Debug.hpp>
#include <Kernel/CPU/CPU.hpp>
#include <Kernel/Display/Screen.hpp>

void    Debug::dump_registers(void)
{
    Screen      cout;
    CPU         cpu = CPU::instantiate();
    Registers   regs = cpu.save_state();

    cout.set_base(BASE::HEX);
    cout << "registers dump:" << "\n";
    cout << "%eax: 0x" << regs.eax << "   %ebx: 0x" << regs.ebx << "   %ecx: 0x" << regs.ecx << "\n";
    cout << "%edx: 0x" << regs.edx << "   %esp: 0x" << regs.esp << "   %ebp: 0x" << regs.ebp << "\n";
    cout << "%esi: 0x" << regs.esi << "   %edi: 0x" << regs.edi << "\n";
    return ;
}

void    Debug::dump_stack(void)
{
    return ;
}
