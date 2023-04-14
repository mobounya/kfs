#include <Kernel/Display/Screen.hpp>

Screen &Screen::operator<<(const char *str)
{
    vga_driver.write_string(str);
    return *this;
}

Screen &Screen::operator<<(const std::string &str)
{
    vga_driver.write_string(str);
    return *this;
}

Screen &Screen::operator<<(int v)
{
    const char *str = itoa(v);
    vga_driver.write_string(str);
    return *this;
}
