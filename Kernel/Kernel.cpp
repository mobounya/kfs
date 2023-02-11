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

extern "C" void kernel_main(void)
{
    VGA::TEXT_MODE vga_interface = VGA::TEXT_MODE();
    vga_interface.write_string("Hello kernel world!\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    vga_interface.write_string((char *)(multiboot_info_ptr->boot_loader_name), VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
}
