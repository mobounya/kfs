#include <stdint.h>
#include <stddef.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/Memory/MemoryManager.hpp>
#include <Kernel/Multiboot/Multiboot.hpp>
#include <Kernel/VGA/VGA.hpp>
#include <Kernel/Memory/MemoryRegion.hpp>
#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/Memory/MemoryPage.hpp>

#include <User/Libc/Libc.hpp>

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

extern "C" void kernel_main(void *page_tables_base_ptr)
{
    Memory::MemoryManager   memory_manager((Memory::PageDirectory *)page_tables_base_ptr);
    VGA::TEXT_MODE          vga;
    uint32_t                mmap_length = multiboot_info_ptr->mmap_length;
    multiboot_mmap          *mmap_addr = multiboot_info_ptr->mmap_addr;
    uint32_t                mmap_structure_size;

    // Setup physical memory regions in the memory manager.
    for (uint32_t i = 0; i < mmap_length; i += mmap_structure_size + 4)
    {
        // TODO: Deal with the other types of memory, maybe declare them to the memory manager as unusable.
        // Only declare available memory to the memory manager.
        if (mmap_addr->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            Memory::MemoryRegion physical_memory_region = Memory::MemoryRegion(mmap_addr->base_addr, mmap_addr->length, mmap_addr->type);
            memory_manager.declare_memory_region(physical_memory_region);
        }

        // Move to the next map buffer, [mmap_addr->size] is the size of the map buffer without [size] itself so we add 4 bytes at the end.
        mmap_structure_size = mmap_addr->size;
        mmap_addr = (multiboot_mmap *)(((uint8_t *)(mmap_addr)) + mmap_structure_size + 4);
    }
    // Allocate one page of physical memory.
    const Memory::MemoryPage *page = memory_manager.allocate_memory_page();

    // Make a page table entry that maps to a memory page.
    Memory::PageTableEntry page_table_entry;
    page_table_entry.set_present()->set_read_write()->set_u_s()->set_pwt()->set_cache_disbled()->set_pat()->set_global();
    page_table_entry.set_physical_address(page->get_base_addr());
    memory_manager.insert_page_table_entry(page_table_entry);

    // Make a page directory entry that referenece a page table.
    Memory::PageDirectoryEntry page_directory_entry;
    page_directory_entry.set_present()->set_read_write()->set_u_s()->set_pwt()->set_cache_disbled()->set_page_size()->set_global();
    page_directory_entry.set_physical_address((uint32_t)memory_manager.get_page_table_ptr());
    memory_manager.insert_page_directory_entry(page_directory_entry);
}
