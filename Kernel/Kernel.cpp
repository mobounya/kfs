#include <stdint.h>
#include <stddef.h>

#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/Memory/PhysicalMemoryManager.hpp>
#include <Kernel/Multiboot/Multiboot.hpp>
#include <Kernel/VGA/VGA.hpp>
#include <Kernel/Memory/MemoryRegion.hpp>
#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/Memory/MemoryPage.hpp>
#include <Kernel/Memory/KernelVirtualMemoryManager.hpp>
#include <Kernel/Memory/UserVirtualMemoryManager.hpp>

#include <string.h>

extern "C" {
    multiboot_info *multiboot_info_ptr;
}

void print_multiboot_info()
{
    VGA::TEXT_MODE &vga_interface = VGA::TEXT_MODE::instantiate();

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

extern "C" void kernel_main(void *kernel_page_tables, void *user_page_tables)
{
    VGA::TEXT_MODE                         &vga = VGA::TEXT_MODE::instantiate();
    Memory::PhysicalMemoryManager          &memory_manager = Memory::PhysicalMemoryManager::instantiate();
    Memory::KernelVirtualMemoryManager     kernel_vm(kernel_page_tables);
    Memory::UserVirtualMemoryManager       user_vm(user_page_tables);
    uint32_t                               mmap_length = multiboot_info_ptr->mmap_length;
    multiboot_mmap                         *mmap_addr = multiboot_info_ptr->mmap_addr;
    uint32_t                               mmap_structure_size;
    const char                             *str = "Allocated kernel memory successfully !\n";

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

    /*
        Physical Memory Mapping:
            - 0x0      --> 0x100000  : 1 Mib, reserved for special usage.
            - 0x100000 --> 0x400000  : 3 Mib, Kernel image (?? I don't know the size of the kernel image).
            - 0x400000 --> 0x800000  : 4 Mib, Available for allocation for the Kernel.
            - 0x800000 --> 0x1000000 : 8 Mib, Available for allocation for user space programs. 
    */

    // Identity map the first 1 Mib (Mebibyte), 0x0 --> 0x100000
    kernel_vm.identity_map_memory(0x0, 0x100000);

    // Identity map the Kernel image, 0x100000 --> 0x400000
    kernel_vm.identity_map_memory(0x100000, 0x400000);

    kernel_vm.load_page_directory();

    // Disable first page so de-refrencing a NULL ptr would not work.
    kernel_vm.disable_page(0x0, PAGE_SIZE);

    memory_manager.enable_paging();

    void *ptr = kernel_vm.allocate_virtual_memory(NULL, PAGE_SIZE, 0);
    if (ptr != NULL)
    {
        memcpy(ptr, str, strlen(str) + 1);
        vga.write_string((char *)ptr, VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
    } else
        vga.write_string("Allocation failed\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::RED, VGA::BLINK::FALSE);
}
