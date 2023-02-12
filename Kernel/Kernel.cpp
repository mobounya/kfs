#include <stdint.h>
#include <stddef.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/Memory/MemoryManager.hpp>
#include <Kernel/Multiboot/Multiboot.hpp>
#include <Kernel/VGA/VGA.hpp>
#include <Kernel/Memory/MemoryRegion.hpp>

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
    Memory::MemoryManager memory_manager;
    VGA::TEXT_MODE vga;
    uint32_t mmap_length = multiboot_info_ptr->mmap_length;
    multiboot_mmap *mmap_addr = multiboot_info_ptr->mmap_addr;
    uint32_t mmap_structure_size;

    // Setup physical memory regions in the memory manager.
    for (uint32_t i = 0; i < mmap_length; i += mmap_structure_size + 4)
    {
        // TODO: Deal with the other types of memory, maybe declare them to the memory manager as unusable.
        // Only declare available memory to the memory manager.
        if (mmap_addr->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            Memory::MemoryRegion physical_memory_region = Memory::MemoryRegion(mmap_addr->base_addr, mmap_addr->length, mmap_addr->type);
            memory_manager.add_physical_memory_region(physical_memory_region);
        }

        // Move to the next map buffer, [mmap_addr->size] is the size of the map buffer without [size] itself so we add 4 bytes at the end.
        mmap_structure_size = mmap_addr->size;
        mmap_addr = (multiboot_mmap *)(((uint8_t *)(mmap_addr)) + mmap_structure_size + 4);
    }
    vga.write_string("Declared avaiable memory to the memory manager !\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
}
