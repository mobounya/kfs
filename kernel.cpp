#include <stdint.h>
#include <stddef.h>
#include "vga_interface.hpp"
#include "not_libc.hpp"

typedef struct my_struct {
    int a = 0x42;
    int b = 0x1337;
    long long c = 0x4242;
    char str[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    VGA::TEXT_MODE vga_interface = VGA::TEXT_MODE();
}   my_struct;

extern "C" void kernel_main(void)
{
    my_struct s;
    void *bp asm ("bp");

    s.vga_interface.write_string("Dumping 360 bytes of the kernel stack starting from stack base pointer:\n\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GRAY, false);
    dump_memory(s.vga_interface, &bp, 360);
}
