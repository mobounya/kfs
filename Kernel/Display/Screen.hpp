#pragma once

#include <stdint.h>
#include <stddef.h>

#include <cstring.h>

#include <Kernel/Klibcpp/string.hpp>
#include <Kernel/Display/VGA/VGA.hpp>

enum BASE
{
    DECIMAL,
    HEX,
};

class Screen
{
    private:
        VGA::TEXT_MODE &vga_driver = VGA::TEXT_MODE::instantiate();
        BASE base;

    public:
        Screen();
        Screen &set_base(BASE base);
        Screen &operator<<(const char *str);
        Screen &operator<<(const std::string &str);
        Screen &operator<<(int v);
};
