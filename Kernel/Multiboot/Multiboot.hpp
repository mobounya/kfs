#pragma once

#include <stdint.h>

#define MULTIBOOT_MEMORY_INFO           (1 << 0)
#define MULTIBOOT_BOOT_DEVICE           (1 << 1)
#define MULTIBOOT_CMDLINE               (1 << 2)
#define MULTIBOOT_MODS                  (1 << 3)

// Bits 4 & 5 are mutually exclusive.
#define MULTIBOOT_SYMS                  (1 << 4)
#define MULTIBOOT_SYMS_ELF              (1 << 5)

#define MULTIBOOT_MEMORY_MAP            (1 << 6)
#define MULTIBOOT_DRIVE                 (1 << 7)
#define MULTIBOOT_CONFIG_TABLE          (1 << 8)
#define MULTIBOOT_BOOT_LOADER_NAME      (1 << 9)
#define MULTIBOOT_APM_TABLE             (1 << 10)
#define MULTIBOOT_VBE_TABLE             (1 << 11)
#define MULTIBOOT_FRAMEBUFFER_TABLE     (1 << 12)

#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5

// Boot information format
// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

struct multiboot_mmap
{
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t  type;
};

struct multiboot_info
{
    // (required)
    uint32_t flags;

    // (present if flags[0] is set)
    uint32_t mem_lower;
    uint32_t mem_upper;

    // (present if flags[1] is set)
    uint32_t boot_device;

    // (present if flags[2] is set)
    uint32_t cmdline;

    // (present if flags[3] is set)
    uint32_t mods_count;
    // TODO: make a struct for this field
    uint32_t mods_addr;

    // TODO: make a struct for this field
    // (present if flags[4] or flags[5] is set)
    uint8_t syms[16];

    // (present if flags[6] is set)
    uint32_t mmap_length;
    multiboot_mmap *mmap_addr;

    // (present if flags[7] is set)
    uint32_t drives_length;
    // TODO: make a struct for this field
    uint32_t drives_addr;

    // (present if flags[8] is set)
    uint32_t config_table;

    // (present if flags[9] is set)
    uint32_t boot_loader_name;

    // TODO: make a struct for this field
    // (present if flags[10] is set)
    uint32_t apm_table;

    // (present if flags[11] is set)
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    // TODO: make a struct for this field
    // (present if flags[12] is set)
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint8_t  color_info[5];
};
