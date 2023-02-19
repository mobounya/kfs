#pragma once

#include <stddef.h>
#include <Kernel/VGA/VGA.hpp>

size_t strlen(const char *str);

int strcmp(const char *s1, const char *s2);

void dump_memory(VGA::TEXT_MODE &vga_interface, void *ptr, size_t size);

char			*itoa(uint64_t nbr);