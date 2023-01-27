#include "not_libc.hpp"

size_t strlen(const char *str)
{
    size_t i = 0;
    for (; str[i] != '\0'; i++);
    return i;
}

int strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    while (s1[i] == s2[i])
        ++i;
    return s1[i] - s2[i];
}

void print_hex(VGA::TEXT_MODE &vga_interface, unsigned char c)
{
    char str[3] = {0,};
    const char *hex = "0123456789abcdef";
    str[0] = hex[c / 16];
    str[1] = hex[c % 16];
    vga_interface.write_string(str, VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GRAY, false);
}

void dump_memory(VGA::TEXT_MODE &vga_interface, void *ptr, size_t size)
{
    unsigned char *data = (unsigned char *)ptr;
    size_t i = 0;
    while (i < size)
    {
        print_hex(vga_interface, data[i]);
        ++i;
        if (i % 16 == 0)
        {
            size_t j = i - 16;
            vga_interface.write_string("                ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::BLACK, false);
            while (j < i)
            {
                char str[2];
                if (data[j] >= 32 && data[j] <= 126)
                    str[0] = data[j];
                else
                    str[0] = '.';
                str[1] = '\0';
                vga_interface.write_string(str, VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GRAY, false);
                j++;
            }
            vga_interface.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::BLACK, false);
        }
        else
            vga_interface.write_string(" ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::BLACK, false);
    }
}
