#include <stdint.h>
#include <stddef.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/Memory/MemoryManager.hpp>

#include <Libc/Libc.hpp>
#include <Kernel/VGA/VGA.hpp>

void setup_kernel()
{
    Memory::MemoryManager memory_manager;
    Memory::page_directory_entry entry;

    entry.set_present()->set_read_write()->set_u_s()->set_cache_disbled()->set_page_size()->set_physical_address(0xfff000);
    memory_manager.insert_page_directory_entry(entry);
}

extern "C" void kernel_main(void)
{
    setup_kernel();
    VGA::TEXT_MODE vga_interface = VGA::TEXT_MODE();
    vga_interface.write_string("Hello kernel !", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
}
