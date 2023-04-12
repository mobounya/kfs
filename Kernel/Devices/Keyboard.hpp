#pragma once

#include <stdint.h>
#include <stddef.h>
#include <ctype.h>

#include <unordered_map.hpp>

class Keyboard
{
    private:
        Keyboard();

    private:
        static bool                             instantiated;
        static Keyboard                         keyboard_driver;
        static std::unordered_map<int, char>    scan_code_set;
        static bool caps_on;

    public:
        static Keyboard         &instantiate(void);
        static int              scan_code(void);
        bool                    is_special_key(int scan_code);
        void                    execute_key(int scan_code);
        char                    get_key_pressed(int scan_code) const;
};
