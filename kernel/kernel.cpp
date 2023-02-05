#include <stdint.h>
#include <stddef.h>
#include "not_libc.hpp"
#include "vga_interface.hpp"

extern "C" void kernel_main(void)
{
    VGA::TEXT_MODE vga_interface = VGA::TEXT_MODE();
    vga_interface.write_string("Hello kernel world !", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
}
