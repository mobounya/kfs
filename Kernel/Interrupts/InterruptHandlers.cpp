#include <Kernel/CPU/CPU.hpp>
#include <Kernel/Display/Screen.hpp>
#include <Kernel/Devices/Keyboard.hpp>

#include <cstring.h>

/*
    - Debug exception
    Vector:      1
    Type:        Fault
    Error code:  No
*/
extern "C" void DB_fault(void)
{
    Screen cout;
    cout << "Debug exception" << "\n";
}

/*
    - Device not available
    Vector:      2
    Type:        Interrupt
    Error code:  No
*/
extern "C" void NMI_interrupt(void)
{
    Screen cout;
    cout << "None maskable interrupt" << "\n";
}

/*
    - Breakpoint
    Vector:      3
    Type:        Trap
    Error code:  No
*/
extern "C" void BP_trap(void)
{
    Screen cout;
    cout << "Breakpoint trap" << "\n";
}

/*
    - Invalid opcode
    Vector:      6
    Type:        Fault
    Error code:  No
*/
extern "C" void UD_fault(void)
{
    Screen cout;
    cout << "Invalid opcode" << "\n";
}

/*
    - Device not available
    Vector:      7
    Type:        fault
    Error code:  No
*/
extern "C" void NM_fault(void)
{
    Screen cout;
    cout << "Device not available" << "\n";
}

/*
    - Double fault
    Vector:      8
    Type:        Abort
    Error code:  Yes (zero)
*/
extern "C" void DF_abort(void)
{
    Screen cout;
    cout << "Double fault" << "\n";
}

/*
    - Invalid TSS
    Vector:      10
    Type:        fault
    Error code:  Yes
*/
extern "C" void TS_fault(void)
{
    Screen cout;
    cout << "Invalid TSS" << "\n";
}

/*
    - Segment not present
    Vector:      11
    Type:        fault
    Error code:  Yes
*/
extern "C" void NP_fault(void)
{
    Screen cout;
    cout << "Segment not present" << "\n";
}

/*
    - Stack segment fault
    Vector:      12
    Type:        fault
    Error code:  Yes
*/
extern "C" void SS_fault(void)
{
    Screen cout;
    cout << "Stack segment fault" << "\n";
}

/*
    - General protection fault
    Vector:      13
    Type:        fault
    Error code:  Yes
*/
extern "C" void GP_fault(void)
{
    Screen cout;
    cout << "General protection fault" << "\n";
}

/*
    - Page fault
    Vector:      14
    Type:        fault
    Error code:  Yes
*/
extern "C" void PF_fault(void)
{
    // TODO: recover from a page fault instead of panicking.
    Screen cout;
    cout << "Page fault" << "\n";
    CPU::panic();
}

/*
    - x87 FPU Floating-point Error (Math fault)
    Vector:      16
    Type:        fault
    Error code:  No
*/
extern "C" void MF_fault(void)
{
    Screen cout;
    cout << "x87 FPU Floating-point Error (Math fault)" << "\n";
}

/*
    - Alignment check
    Vector:      17
    Type:        fault
    Error code:  Yes (zero)
*/
extern "C" void AC_fault(void)
{
    Screen cout;
    cout << "Alignment check" << "\n";
}

/*
    - Machine check
    Vector:      18
    Type:        abort
    Error code:  No
*/
extern "C" void MC_fault(void)
{
    Screen cout;
    cout << "Machine check" << "\n";
}

/*
    - SIMD Floating-point Error (Math fault)
    Vector:      19
    Type:        abort
    Error code:  No
*/
extern "C" void XM_fault(void)
{
    Screen cout;
    cout << "SIMD Floating-point Error (Math fault)" << "\n";
}

extern "C" void keyboard_handler(void)
{
    Screen cout;
    char str[2] = { '\0' };
    Keyboard    &keyboard_driver = Keyboard::instantiate();
    int scan_code = keyboard_driver.scan_code();
    // F12 panics the kernel, used for debugging.
    if (scan_code == 0x58)
        CPU::panic();
    else if (keyboard_driver.is_special_key(scan_code) == true)
        keyboard_driver.execute_key(scan_code);
    else
    {
        str[0] = keyboard_driver.get_key_pressed(scan_code);
        cout << str;
    }
    CPU::outb(0x20, 0x20);
}

extern "C" void default_handler(void)
{
    Screen cout;
    cout << "Called default handler" << "\n";
}
