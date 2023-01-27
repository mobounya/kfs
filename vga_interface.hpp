#pragma once

#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_BUFFER_SIZE (VGA_WIDTH * VGA_HEIGHT)

typedef uint16_t vga_char;
typedef uint8_t vga_attribute;

namespace VGA {

/*
 * https://en.wikipedia.org/wiki/List_of_8-bit_computer_hardware_graphics#IBM_PC/XT_and_compatible_systems#CGA
 * List of available colors.
 * */

enum FG_COLOR {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    YELLOW,
    GRAY,
    LIGHT_GRAY,
    LIGHT_BLUE,
    LIGHT_BLACK,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_YELLOW,
};

/* Same as VGA_FG_COLOR colors, except that background colors don't have light versions */
enum BG_COLOR {
    BG_BLACK,
    BG_BLUE,
    BG_GREEN,
    BFG_CYAN,
    BG_RED,
    BG_MAGENTA,
    BG_YELLOW,
    BG_GRAY,
};

enum BLINK {
    FALSE,
    TRUE,
};

class TEXT_MODE
{
    private:
        vga_char screen_buffer[VGA_BUFFER_SIZE];
        vga_char *vga_buffer;
        size_t current_index = 0;

        void init_screen_buffer();
        void write_char(vga_char c);
        void flush_buffer_to_screen();
        void shift_screen_buffer(size_t size);
    public:
        TEXT_MODE();
        void set_start(size_t y, size_t x);
        void write_string(const char *string, BG_COLOR bg_color, FG_COLOR fg_color, bool blink);
        vga_char create_char(vga_attribute attr, char ascii_char);
        vga_attribute set_foreground_color(vga_attribute attr, FG_COLOR color);
        vga_attribute set_background_color(vga_attribute attr, BG_COLOR color);
        vga_attribute set_blink(vga_attribute attr);
};
}
