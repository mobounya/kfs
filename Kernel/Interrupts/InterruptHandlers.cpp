#include <Kernel/CPU/CPU.hpp>
#include <Kernel/VGA/VGA.hpp>
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
    VGA::TEXT_MODE  &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Debug exception\n");
}

/*
    - Device not available
    Vector:      2
    Type:        Interrupt
    Error code:  No
*/
extern "C" void NMI_interrupt(void)
{
    VGA::TEXT_MODE  &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("None maskable interrupt\n");
}

/*
    - Breakpoint
    Vector:      3
    Type:        Trap
    Error code:  No
*/
extern "C" void BP_trap(void)
{
    VGA::TEXT_MODE  &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Breakpoint trap\n");
}

/*
    - Invalid opcode
    Vector:      6
    Type:        Fault
    Error code:  No
*/
extern "C" void UD_fault(void)
{
    VGA::TEXT_MODE  &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Invalid opcode\n");
}

/*
    - Device not available
    Vector:      7
    Type:        fault
    Error code:  No
*/
extern "C" void NM_fault(void)
{
    VGA::TEXT_MODE  &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Device not available\n");
}

/*
    - Double fault
    Vector:      8
    Type:        Abort
    Error code:  Yes (zero)
*/
extern "C" void DF_abort(void)
{
    VGA::TEXT_MODE  &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Double fault\n");
}

/*
    - Invalid TSS
    Vector:      10
    Type:        fault
    Error code:  Yes
*/
extern "C" void TS_fault(void)
{
    VGA::TEXT_MODE  &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Invalid TSS\n");
}

/*
    - Segment not present
    Vector:      11
    Type:        fault
    Error code:  Yes
*/
extern "C" void NP_fault(void)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Segment not present\n");
}

/*
    - Stack segment fault
    Vector:      12
    Type:        fault
    Error code:  Yes
*/
extern "C" void SS_fault(void)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Stack segment fault\n");
}

/*
    - General protection fault
    Vector:      13
    Type:        fault
    Error code:  Yes
*/
extern "C" void GP_fault(void)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("General protection fault\n");
}

/*
    - Page fault
    Vector:      14
    Type:        fault
    Error code:  Yes
*/
extern "C" void PF_fault(void)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Page fault\n");
}

/*
    - x87 FPU Floating-point Error (Math fault)
    Vector:      16
    Type:        fault
    Error code:  No
*/
extern "C" void MF_fault(void)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("x87 FPU Floating-point Error (Math fault)\n");
}

/*
    - Alignment check
    Vector:      17
    Type:        fault
    Error code:  Yes (zero)
*/
extern "C" void AC_fault(void)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Alignment check\n");
}

/*
    - Machine check
    Vector:      18
    Type:        abort
    Error code:  No
*/
extern "C" void MC_fault(void)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Machine check\n");
}

/*
    - SIMD Floating-point Error (Math fault)
    Vector:      19
    Type:        abort
    Error code:  No
*/
extern "C" void XM_fault(void)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("SIMD Floating-point Error (Math fault)\n");
}

extern "C" void keyboard_handler(void)
{
    char str[2] = { '\0' };
    VGA::TEXT_MODE  &vga = VGA::TEXT_MODE::instantiate();
    Keyboard    &keyboard_driver = Keyboard::instantiate();
    int scan_code = keyboard_driver.scan_code();
    if (keyboard_driver.is_special_key(scan_code) == true)
        keyboard_driver.execute_key(scan_code);
    else
    {
        str[0] = keyboard_driver.get_key_pressed(scan_code);
        vga.write_string(str);
    }
    CPU::outb(0x20, 0x20);
}

extern "C" void default_handler(void)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Called default handler\n");
    // vga.write_string("ip: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string(itoa(frame->ip), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string("cs: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string(itoa(frame->cs), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string("sp: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string(itoa(frame->sp), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string("ss: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string(itoa(frame->ss), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    // vga.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
}
