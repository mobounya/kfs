#pragma once

#include <stdint.h>
#include <stddef.h>

#include <cstring.h>

#include <Kernel/Klibcpp/string.hpp>
#include <Kernel/Display/VGA/VGA.hpp>

class Screen
{
    private:
        VGA::TEXT_MODE &vga_driver = VGA::TEXT_MODE::instantiate();

    public:
        Screen &operator<<(const char *str);
        Screen &operator<<(const std::string &str);
        Screen &operator<<(int v);
};
