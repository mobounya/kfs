#pragma once

#include <stdint.h>
#include <stddef.h>

/* 
    Note 1: we're not dumping segment registers since we're not using segmentation.
    Note 2: we're on IA-32 mode, which means registers are 32 bit.
    General registers: %eax %ebx %ecx %edx %esp %ebp %esi %edi
*/

struct Registers
{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;  
};
