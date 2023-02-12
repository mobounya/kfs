#include <stdint.h>
#include <stddef.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/Memory/MemoryManager.hpp>
#include <Kernel/Multiboot/Multiboot.hpp>
#include <Kernel/VGA/VGA.hpp>

#include <Libc/Libc.hpp>

extern "C" {
    multiboot_info *multiboot_info_ptr;
}

void print_multiboot_info()
{
    VGA::TEXT_MODE vga_interface = VGA::TEXT_MODE();
    if (multiboot_info_ptr->flags & MULTIBOOT_BOOT_LOADER_NAME) {
        vga_interface.write_string("Loaded by:\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GREEN, VGA::BLINK::FALSE);
        vga_interface.write_string("- ", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GREEN, VGA::BLINK::FALSE);
        vga_interface.write_string((char *)(multiboot_info_ptr->boot_loader_name), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GREEN, VGA::BLINK::FALSE);
        vga_interface.write_string("\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GREEN, VGA::BLINK::FALSE);
    }
    if (multiboot_info_ptr->flags & MULTIBOOT_MEMORY_INFO)
        vga_interface.write_string("Memory info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    if (multiboot_info_ptr->flags & MULTIBOOT_BOOT_DEVICE)
        vga_interface.write_string("Boot device info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    if (multiboot_info_ptr->flags & MULTIBOOT_CMDLINE)
        vga_interface.write_string("Command line info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    if (multiboot_info_ptr->flags & MULTIBOOT_MODS)
        vga_interface.write_string("Boot modules info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    if (multiboot_info_ptr->flags & MULTIBOOT_SYMS)
        vga_interface.write_string("SYMS info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    if (multiboot_info_ptr->flags & MULTIBOOT_MEMORY_MAP)
        vga_interface.write_string("Memory map info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    if (multiboot_info_ptr->flags & MULTIBOOT_DRIVE)
        vga_interface.write_string("Drives info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    if (multiboot_info_ptr->flags & MULTIBOOT_CONFIG_TABLE)
        vga_interface.write_string("ROM configuration info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    if (multiboot_info_ptr->flags & MULTIBOOT_APM_TABLE)
        vga_interface.write_string("APM info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    if (multiboot_info_ptr->flags & MULTIBOOT_VBE_TABLE)
        vga_interface.write_string("VBE info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    if (multiboot_info_ptr->flags & MULTIBOOT_FRAMEBUFFER_TABLE)
        vga_interface.write_string("FRAMEBUFFER info available\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
}

extern "C" void kernel_main(void)
{
    uint32_t size = multiboot_info_ptr->mmap_addr->size; // 0x14
    uint64_t base_addr = multiboot_info_ptr->mmap_addr->base_addr; // 0x0 = 0
    uint64_t length = multiboot_info_ptr->mmap_addr->length; // 0x9fc00 = 654336
    uint32_t  type = multiboot_info_ptr->mmap_addr->type; // 0x1 = RAM

    // uint32_t mmap_length = multiboot_info_ptr->mmap_length; // 0x90
    // uint32_t mem_lower = multiboot_info_ptr->mem_lower; // 0x27f
    // uint32_t mem_upper = multiboot_info_ptr->mem_upper; // 0x1fb80
}
