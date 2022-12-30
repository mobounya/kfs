#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t *vga_buffer = (uint16_t *)0xB8000;

typedef uint16_t vga_char;
typedef uint8_t vga_attribute;

/*
 * https://en.wikipedia.org/wiki/List_of_8-bit_computer_hardware_graphics#IBM_PC/XT_and_compatible_systems#CGA
 * List of available colors.
 * */

typedef enum VGA_COLOR {
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
    LIGHT_YELLOW
} VGA_COLOR;

typedef enum bool {
    FALSE,
    TRUE
} bool;

static size_t current_index = 0;

size_t ft_strlen(const char *str)
{
    size_t i = 0;
    for (; str[i] != '\0'; i++);
    return i;
}

void vga_write_char(vga_char c, size_t y, size_t x)
{
    size_t index = (y * VGA_WIDTH) + x;
    vga_buffer[index] = c;
}

vga_attribute vga_set_foreground_color(vga_attribute attr, VGA_COLOR color)
{
    attr |= color;
    return attr;
}

vga_attribute vga_set_background_color(vga_attribute attr, VGA_COLOR color)
{
    attr |= color << 4;
    return attr;
}

vga_attribute  vga_set_blink(vga_attribute attr)
{
    attr |= 0x80; // MSB is the blink bit
    return attr;
}

vga_char vga_create_char(vga_attribute attr, char ascii_char)
{
    vga_char c = ((uint16_t)attr << 8) | ascii_char;
    return c;
}

void vga_write_string(const char *string, VGA_COLOR bg_color, VGA_COLOR fg_color, bool blink)
{
    for (size_t i = 0; string[i] != '\0'; i++)
    {
        vga_attribute attr = 0;
        attr = vga_set_foreground_color(attr, fg_color);
        attr = vga_set_background_color(attr, bg_color);
        if (blink == TRUE)
            attr = vga_set_blink(attr);
        vga_char c = vga_create_char(attr, string[i]);
        vga_write_char(c, current_index / VGA_WIDTH, current_index % VGA_WIDTH);
        ++current_index;
        if (current_index == VGA_WIDTH * VGA_HEIGHT)
            current_index = 0;
    }
}

void kernel_main(void)
{
    const char *str = "42";

    current_index = (VGA_WIDTH * (VGA_HEIGHT / 2)) + (VGA_WIDTH / 2) - ft_strlen(str);
    vga_write_string(str, LIGHT_YELLOW, BLACK, TRUE);
}
