#include <stdint.h>
#include <stddef.h>
#include "vga_interface.hpp"
#include "not_libc.hpp"

extern "C" void kernel_main(void)
{
    const char *str[] = {
        "        :::      ::::::::",
        "      :+:      :+:    :+:",
        "    +:+ +:+         +:+  ",
        "  +#+  +:+       +#+     ",
        "+#+#+#+#+#+   +#+        ",
        "     #+#    #+#          ",
        "    ###   ########.fr    "
    };
    const size_t size = sizeof(str) / sizeof(const char *);
    VGA::TEXT_MODE vga_interface = VGA::TEXT_MODE();

    for (size_t i = 0; i < size; ++i)
    {
        vga_interface.set_start((VGA_HEIGHT - size) / 2 + i, (VGA_WIDTH / 2) - (ft_strlen(str[i]) / 2));
        vga_interface.write_string(str[i], VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::TRUE);
    }
}
