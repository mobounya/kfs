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
#include <Kernel/GDT/GDT.hpp>
#include <Kernel/GDT/TSS.hpp>

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

extern "C" void setup_gdt(void *stack_ptr)
{
    Kernel::GDT_Table   *gdt_table_ptr = (Kernel::GDT_Table *)0x00000800;
    // Store the tss right after the GDT.
    Kernel::TSS         *tss_ptr = (Kernel::TSS *)(((Kernel::GDT_Table *)0x00000800) + 1);
    Kernel::GDT_Table gdt_table;

    size_t  gdt_size = 0;
    uint8_t access_byte = PRESENT_SEGMENT | CODE_DATA_SEGMENT | EXECUTABLE_SEGMENT | ALLOW_READ_ACCESS;
    uint8_t flags = GRANUALITY_FLAG | SEGMENT_32BIT_FLAG;

    memset(gdt_table_ptr, 0x0, sizeof(Kernel::GDT_Table));

    // NULL segment.
    gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor(0x0, 0x0, 0x0, 0x0), gdt_size++);

    // Segment descriptor for kernel code.
    gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor(0x0, 0xfffff, access_byte, flags), gdt_size++);

    // Segment descriptor for kernel data.
    access_byte = PRESENT_SEGMENT | CODE_DATA_SEGMENT | ALLOW_WRITE_ACCESS;
    gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor(0x0, 0xfffff, access_byte, flags), gdt_size++);

    // Segment descriptor for kernel stack.
    access_byte = PRESENT_SEGMENT | CODE_DATA_SEGMENT | ALLOW_WRITE_ACCESS;
    gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor(0x0, 0xfffff, access_byte, flags), gdt_size++);

    // Segment descriptor for user code.
    access_byte = PRESENT_SEGMENT | DPL_USER | CODE_DATA_SEGMENT | EXECUTABLE_SEGMENT | ALLOW_READ_ACCESS;
    gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor(0x0, 0xfffff, access_byte, flags), gdt_size++);

    // Segment descriptor for user data.
    access_byte = PRESENT_SEGMENT | DPL_USER | CODE_DATA_SEGMENT | ALLOW_WRITE_ACCESS;
    gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor(0x0, 0xfffff, access_byte, flags), gdt_size++);

    // Segment descriptor for user stack.
    access_byte = PRESENT_SEGMENT | DPL_USER | CODE_DATA_SEGMENT | ALLOW_WRITE_ACCESS;
    gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor(0x0, 0xfffff, access_byte, flags), gdt_size++);

    // Setup the tss.
    memset(tss_ptr, 0x0, sizeof(Kernel::TSS));
    tss_ptr->set_SS0(0x10);
    tss_ptr->set_ESP0((uint32_t)stack_ptr);
    tss_ptr->set_IOPB(sizeof(Kernel::TSS) - 4);

    // System segment descriptor for TSS.
    access_byte = 0x89;
    flags = 0x8;
    // https://wiki.osdev.org/Task_State_Segment
    // IOPB may get the value sizeof(TSS) (which is 104) if you don't plan to use this io-bitmap further (according to mystran in http://forum.osdev.org/viewtopic.php?t=13678)
    gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor((uint32_t)tss_ptr, sizeof(Kernel::TSS), access_byte, flags), gdt_size++);

    *gdt_table_ptr = gdt_table;
}

extern "C" void kernel_main(void *kernel_page_tables, void *user_page_tables, void *interrupt_descriptor_table_ptr, void *idt_descriptor_ptr)
{
    VGA::TEXT_MODE                          &vga = VGA::TEXT_MODE::instantiate();
    Memory::PhysicalMemoryManager           &memory_manager = Memory::PhysicalMemoryManager::instantiate();
    Memory::KernelVirtualMemoryManager      kernel_vm(kernel_page_tables);
    Memory::UserVirtualMemoryManager        user_vm(user_page_tables);
    uint32_t                                mmap_length = multiboot_info_ptr->mmap_length;
    multiboot_mmap                          *mmap_addr = multiboot_info_ptr->mmap_addr;
    uint32_t                                mmap_structure_size;

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
    // kernel_vm.identity_map_memory(0x0, 0x100000);

    // Identity map the Kernel image, 0x100000 --> 0x400000
    // kernel_vm.identity_map_memory(0x100000, 0x400000);

    // kernel_vm.load_page_directory();

    // memory_manager.enable_paging();

    // Disable first page so de-refrencing a NULL ptr would not work.
    // kernel_vm.disable_page(0x0, PAGE_SIZE);

    vga.write_string("Kernel Done !\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GREEN, VGA::BLINK::FALSE);
}
