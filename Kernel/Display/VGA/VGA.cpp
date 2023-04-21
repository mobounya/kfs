#include <Kernel/Display/VGA/VGA.hpp>
#include <Kernel/CPU/CPU.hpp>
#include <cstring.h>

namespace VGA
{
    bool            TEXT_MODE::instantiated = false;
    TEXT_MODE       TEXT_MODE::instance;

    TEXT_MODE::TEXT_MODE()
    {
        this->vga_buffer = (uint16_t *)0xB8000;
        this->current_index = 0;
    }

    TEXT_MODE &TEXT_MODE::instantiate(void)
    {
        if (instantiated == false)
        {
            instance = TEXT_MODE();
            instantiated = true;
        }
        return instance;
    }

    void TEXT_MODE::set_start(size_t y, size_t x)
    {
        this->current_index = (y * VGA_WIDTH) + x;
    }

    void TEXT_MODE::write_char(vga_char c)
    {
        vga_buffer[current_index] = c;
    }

    void TEXT_MODE::write_string(const char *string, BG_COLOR bg_color, FG_COLOR fg_color, bool blink)
    {
        for (size_t i = 0; string[i] != '\0'; i++)
        {
            vga_attribute attr = 0;
            attr = set_foreground_color(attr, fg_color);
            attr = set_background_color(attr, bg_color);
            if (blink == true)
                attr = set_blink(attr);
            vga_char c = create_char(attr, string[i]);
            if (string[i] == '\n')
            {
                current_index = (current_index / VGA_WIDTH + 1) * VGA_WIDTH; 
            }
            else
            {
                this->write_char(c);
                current_index++;
            }
        }
    }


    void TEXT_MODE::write_string(const std::string &str, BG_COLOR bg_color, FG_COLOR fg_color, bool blink)
    {
        write_string(str.c_str(), bg_color, fg_color, blink);
    }

    void TEXT_MODE::write_string(const char *string)
    {
        bool blink = false;
        write_string(string, BG_COLOR::BG_BLACK, FG_COLOR::LIGHT_YELLOW, blink);
    }

    void TEXT_MODE::write_string(const std::string &str)
    {
        bool blink = false;
        write_string(str, BG_COLOR::BG_BLACK, FG_COLOR::LIGHT_YELLOW, blink);
    }

    vga_char TEXT_MODE::create_char(vga_attribute attr, char ascii_char)
    {
        vga_char c = ((uint16_t)attr << 8) | ascii_char;
        return c;
    }

    vga_attribute TEXT_MODE::set_foreground_color(vga_attribute attr, FG_COLOR color)
    {
        attr |= color;
        return attr;
    }

    vga_attribute TEXT_MODE::set_background_color(vga_attribute attr, BG_COLOR color)
    {
        attr |= color << 4;
        return attr;
    }

    vga_attribute TEXT_MODE::set_blink(vga_attribute attr)
    {
        attr |= 0x80; // MSB is the blink bit
        return attr;
    }
}
