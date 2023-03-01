#include <Kernel/VGA/VGA.hpp>
#include <string.h>

namespace VGA
{
    bool            TEXT_MODE::instantiated = false;
    TEXT_MODE       TEXT_MODE::instance;

    TEXT_MODE::TEXT_MODE()
    {
        this->vga_buffer = (uint16_t *)0xB8000;
        this->current_index = 0;
        init_screen_buffer();
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

    void  TEXT_MODE::init_screen_buffer()
    {
        vga_attribute attr = 0;
        attr = set_background_color(attr, BG_COLOR::BG_BLACK);
        attr = set_foreground_color(attr, FG_COLOR::BLACK);
        vga_char c = create_char(attr, ' ');
        for (size_t y = 0; y < VGA_HEIGHT; y++)
        {
            for (size_t x = 0; x < VGA_WIDTH; x++)
                screen_buffer[y * VGA_WIDTH + x]= c;
        }
    }

    void TEXT_MODE::flush_buffer_to_screen()
    {
        for (size_t i = 0; i < VGA_BUFFER_SIZE; i++)
            vga_buffer[i] = screen_buffer[i];
    }

    void TEXT_MODE::shift_screen_buffer(size_t size)
    {
        // We already have enough space, don't do anything.
        if (current_index + size < VGA_BUFFER_SIZE)
            return;

        // calculate how many bytes do we need to reserve.
        size = (current_index + size) % VGA_BUFFER_SIZE;
    
        size_t index = size;
        for (size_t i = 0; index < current_index; i++, index++)
            screen_buffer[i] = screen_buffer[index];
        current_index -= size;
    }

    void TEXT_MODE::set_start(size_t y, size_t x)
    {
        this->current_index = (y * VGA_WIDTH) + x;
    }

    void TEXT_MODE::write_char(vga_char c)
    {
        screen_buffer[current_index] = c;
    }

    void TEXT_MODE::write_string(const char *string, BG_COLOR bg_color, FG_COLOR fg_color, bool blink)
    {
        shift_screen_buffer(strlen(string));
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
        flush_buffer_to_screen();
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
