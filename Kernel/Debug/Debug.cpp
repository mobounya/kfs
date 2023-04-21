#include <Kernel/Debug/Debug.hpp>
#include <Kernel/CPU/CPU.hpp>
#include <Kernel/Display/Screen.hpp>

extern "C"
{
    const void    *stack_top_ptr;
}

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

void    Debug::print_mem_line(void *ptr, size_t size)
{
    Screen cout;
    cout.set_base(BASE::HEX);

    uint8_t *temp = (uint8_t *)ptr;

    size_t i = 0;
    while (i < size)
    {
        cout << "0x" << (uint32_t)temp << "  ";
        for (size_t j = 0; j < size && j < 16; j++)
        {
            if ((int)(temp[i + j]) > 0xf)
                cout << (int)(temp[i + j]) << " ";
            else
                cout << "0" << (int)(temp[i + j]) << " ";
            if (j == 7 || j == 15)
                cout << " ";
        }
        i += 16;
        cout << "\n";
    }
}

void    Debug::dump_stack(void)
{
    // Each line will display 16 bytes, so let's display 15 lines.
    uint8_t max_bytes = (16 * 15);
    size_t printed_bytes = 0;
    Screen  cout;
    CPU     cpu = CPU::instantiate();
    cout.set_base(BASE::HEX);

    uint8_t *stack_start = (uint8_t *)cpu.get_esp();

    cout << "\nStack dump: " << "\n";
    while (printed_bytes < max_bytes && (uint32_t)stack_start < (uint32_t)stack_top_ptr)
    {
        size_t memory_left = (uint32_t)stack_top_ptr - (uint32_t)stack_start;
        size_t bytes_left = (max_bytes - printed_bytes);
        size_t bytes_to_print = 0;
        if (memory_left < 16)
            bytes_to_print = memory_left;
        else if (memory_left >= 16)
            bytes_to_print = 16;
        if (bytes_left < bytes_to_print)
            bytes_to_print = bytes_left;
        Debug::print_mem_line(stack_start, bytes_to_print);
        stack_start += bytes_to_print;
        printed_bytes += bytes_to_print;
    }
    return ;
}

void Debug::print_mem(void *ptr, size_t size)
{
    Screen  cout;
    uint8_t max_bytes = size;
    size_t printed_bytes = 0;
    cout.set_base(BASE::HEX);

    uint8_t *stack_start = (uint8_t *)ptr;

    while (printed_bytes < max_bytes)
    {
        size_t bytes_left = (max_bytes - printed_bytes);
        size_t bytes_to_print = 0;
        if (bytes_left < 16)
            bytes_to_print = bytes_left;
        else if (bytes_left >= 16)
            bytes_to_print = 16;
        Debug::print_mem_line(stack_start, bytes_to_print);
        stack_start += bytes_to_print;
        printed_bytes += bytes_to_print;
    }
    return ;
}