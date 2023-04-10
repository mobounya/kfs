#include <Kernel/CPU/CPU.hpp>
#include <Kernel/VGA/VGA.hpp>
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
    vga.write_string("Debug exception\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("None maskable interrupt\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("Breakpoint trap\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("Invalid opcode\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("Device not available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("Double fault\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("Invalid TSS\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("Segment not present\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("Stack segment fault\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("General protection fault\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("Page fault\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("x87 FPU Floating-point Error (Math fault)\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("Alignment check\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("Machine check\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
    vga.write_string("SIMD Floating-point Error (Math fault)\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
}

extern "C" void keyboard_handler(void)
{
    unsigned char scan_code = CPU::inb(0x60);
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Keyboard Interrupt: ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
    vga.write_string(itoa(scan_code), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
    vga.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
    CPU::outb(0x20, 0x20);
}

extern "C" void default_handler(void)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Called default handler\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::YELLOW, VGA::BLINK::FALSE);
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
