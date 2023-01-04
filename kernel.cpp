#include <stdint.h>
#include <stddef.h>
#include "vga_interface.hpp"
#include "not_libc.hpp"

size_t ft_strlen(const char *str)
{
    size_t i = 0;
    for (; str[i] != '\0'; i++);
    return i;
}

extern "C" void kernel_main(void)
{
    const char *str = "42";
    VGA::TEXT_MODE vga_interface = VGA::TEXT_MODE();

    vga_interface.set_start(VGA_HEIGHT / 2, (VGA_WIDTH / 2) - (ft_strlen(str) / 2));
    vga_interface.write_string(str, VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::TRUE);
}
