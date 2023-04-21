#pragma once

#include <stdint.h>
#include <stddef.h>
#include <ctype.h>

#include <unordered_map.hpp>

typedef void (*key_handler)(void);

class Keyboard
{
    private:
        Keyboard();

    private:
        static bool                                     instantiated;
        static Keyboard                                 keyboard_driver;
        static std::unordered_map<int, char>            scan_code_set;
        static std::unordered_map<int, key_handler>     key_handlers;
        static bool caps_on;

    public:
        static Keyboard         &instantiate(void);
        static int              scan_code(void);
        bool                    is_special_key(int scan_code);
        void                    execute_key(int scan_code);
        char                    get_key_pressed(int scan_code) const;
        void                    map_key(int key_code, key_handler fn);
        key_handler             get_key_handler(int key_code);
};
