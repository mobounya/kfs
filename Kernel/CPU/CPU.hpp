#pragma once

#include <stdint.h>
#include <stddef.h>

class CPU
{
    public:
        static void      outb(uint16_t port, uint8_t val);
        static uint8_t   inb(uint16_t port);
        static void      io_wait(void);
};
