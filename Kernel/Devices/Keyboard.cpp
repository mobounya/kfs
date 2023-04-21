#include <Kernel/Devices/Keyboard.hpp>
#include <Kernel/CPU/CPU.hpp>

bool                                    Keyboard::instantiated = false;
Keyboard                                Keyboard::keyboard_driver;
std::unordered_map<int, char>           Keyboard::scan_code_set;
std::unordered_map<int, key_handler>    Keyboard::key_handlers;
bool                                    Keyboard::caps_on = false;

Keyboard::Keyboard()
{
    scan_code_set = std::unordered_map<int, char>();
    key_handlers = std::unordered_map<int, key_handler>();
    scan_code_set[0x01] = '?'; // escape
    scan_code_set[0x02] = '1';
    scan_code_set[0x03] = '2';
    scan_code_set[0x04] = '3';
    scan_code_set[0x05] = '4';
    scan_code_set[0x06] = '5';
    scan_code_set[0x07] = '6';
    scan_code_set[0x08] = '7';
    scan_code_set[0x09] = '8';
    scan_code_set[0x0A] = '9';
    scan_code_set[0x0B] = '0';
    scan_code_set[0x0C] = '-';
    scan_code_set[0x0D] = '=';
    scan_code_set[0x0E] = '\0'; // Backspace
    scan_code_set[0x0F] = '\0'; // Tab
    scan_code_set[0x10] = 'q';
    scan_code_set[0x11] = 'w';
    scan_code_set[0x12] = 'e';
    scan_code_set[0x13] = 'r';
    scan_code_set[0x14] = 't';
    scan_code_set[0x15] = 'y';
    scan_code_set[0x16] = 'u';
    scan_code_set[0x17] = 'i';
    scan_code_set[0x18] = 'o';
    scan_code_set[0x19] = 'p';
    scan_code_set[0x1A] = '[';
    scan_code_set[0x1B] = ']';
    scan_code_set[0x1C] = '\n'; // Enter
    scan_code_set[0x1D] = '\0'; // Left control 
    scan_code_set[0x1E] = 'a';
    scan_code_set[0x1F] = 's';
    scan_code_set[0x20] = 'd';
    scan_code_set[0x21] = 'f';
    scan_code_set[0x22] = 'g';
    scan_code_set[0x23] = 'h';
    scan_code_set[0x24] = 'j';
    scan_code_set[0x25] = 'k';
    scan_code_set[0x26] = 'l';
    scan_code_set[0x27] = ';';
    scan_code_set[0x28] = '\'';
    scan_code_set[0x29] = '`';
    scan_code_set[0x2A] = '\0'; // left shift
    scan_code_set[0x2B] = '\\';
    scan_code_set[0x2C] = 'z';
    scan_code_set[0x2D] = 'x';
    scan_code_set[0x2E] = 'c';
    scan_code_set[0x2F] = 'v';
    scan_code_set[0x30] = 'b';
    scan_code_set[0x31] = 'n';
    scan_code_set[0x32] = 'm';
    scan_code_set[0x33] = ',';
    scan_code_set[0x34] = '.';
    scan_code_set[0x35] = '/';
    scan_code_set[0x36] = '\0'; // right shift
    scan_code_set[0x37] = '*';
    scan_code_set[0x38] = '\0'; // left alt
    scan_code_set[0x39] = ' ';
    scan_code_set[0x3A] = '\0'; // CapsLock pressed
    scan_code_set[0x58] = '\0'; // F12 pressed
}

Keyboard       &Keyboard::instantiate(void)
{
    if (instantiated == false)
    {
        keyboard_driver = Keyboard();
        instantiated = true;
    }
    return keyboard_driver;
}

// Special keys are the keys that are not associated with a character we can print on screen
// but rather we need to do something else, the caps key for example.
bool            Keyboard::is_special_key(int scan_code)
{
    if (scan_code == 0x0E || scan_code == 0x0F || scan_code == 0x1D || scan_code == 0x2A || scan_code == 0x36 
        || scan_code == 0x38 || scan_code == 0x3A)
        return true;
    else
        return false;
}

void            Keyboard::execute_key(int scan_code)
{
    if (is_special_key(scan_code) == false)
        return ;
    if (scan_code == 0x3A)
        caps_on = !(caps_on);
}

char        Keyboard::get_key_pressed(int scan_code) const
{
    if (scan_code_set.contains(scan_code) == true)
    {
        char c = scan_code_set.at(scan_code);
        if (isalpha(c) == true)
            return (caps_on == false) ? c : toupper(c);
        else
            return c;
    }
    else
        return '\0';
}

int         Keyboard::scan_code(void)
{
    return CPU::inb(0x60);
}

void        Keyboard::map_key(int key_code, key_handler fn)
{
    key_handlers[key_code] = fn;
}

key_handler Keyboard::get_key_handler(int key_code)
{
    if (key_handlers.contains(key_code))
        return key_handlers[key_code];
    else
        return NULL;
}
