#include <Kernel/Display/Screen.hpp>
#include <format.hpp>

Screen::Screen()
{
    set_base(BASE::DECIMAL);
}

Screen &Screen::set_base(BASE base)
{
    this->base = base;
    return *this;
}

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
    const char *str;

    if (this->base == BASE::DECIMAL)
        str = itoa(v);
    else
        str = to_hex(v);
    vga_driver.write_string(str);
    return *this;
}
