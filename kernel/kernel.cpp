#include <stdint.h>
#include <stddef.h>
#include "not_libc.hpp"
#include "vga_interface.hpp"
#include "paging_setup.hpp"

void setup_kernel()
{
    paging_structure_entry entry = create_page_directory_entry(true, true, true, false, true, false, 0xfffff000);
    insert_page_directory_entry(entry);
}

extern "C" void kernel_main(void)
{
    setup_kernel();
    VGA::TEXT_MODE vga_interface = VGA::TEXT_MODE();
    vga_interface.write_string("Hello kernel world !", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
}
