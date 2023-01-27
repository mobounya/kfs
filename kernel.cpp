#include <stdint.h>
#include <stddef.h>
#include "vga_interface.hpp"
#include "not_libc.hpp"


extern "C" void kernel_main(void)
{
    char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    long c = 4242;
    int a = 42;
    int b = 1337;

    VGA::TEXT_MODE vga_interface = VGA::TEXT_MODE();
    vga_interface.write_string("Dumping last 368 bytes of the stack starting from b:\n\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GRAY, false);
    dump_memory(vga_interface, ((void *)&b), 368);
}
