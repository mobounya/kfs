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
#include <Kernel/Interrupts/PIC.hpp>
#include <Kernel/Interrupts/IDT.hpp>

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

struct interrupt_frame {
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
} __attribute__((packed));
 
__attribute__((interrupt)) void interrupt_handler(struct interrupt_frame* frame)
{
    VGA::TEXT_MODE &vga = VGA::TEXT_MODE::instantiate();
    vga.write_string("Interrupt routine called\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GREEN, VGA::BLINK::FALSE);
    return ;
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
    Interrupts::InterruptDescriptorTable    *interrupt_descriptor_table = (Interrupts::InterruptDescriptorTable *)interrupt_descriptor_table_ptr;
    Interrupts::IDTDescriptor               *idt_descriptor = (Interrupts::IDTDescriptor *)idt_descriptor_ptr;

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

    // kernel_vm.load_page_directory();

    // memory_manager.enable_paging();

    // Disable first page so de-refrencing a NULL ptr would not work.
    // kernel_vm.disable_page(0x0, PAGE_SIZE);

    Interrupts::GateDescriptor descriptor;
    descriptor.set_offset((uint32_t)&interrupt_handler);
    descriptor.set_segment_selector(1);
    descriptor.set_gate_type(Interrupts::GateType::InterruptGate32);
    descriptor.set_DPL(0);
    descriptor.set_present(true);

    for (int i = 0; i < IDT_SIZE; i++)
        interrupt_descriptor_table->insert_new_entry(descriptor, i);

    idt_descriptor->set_address((uint32_t)interrupt_descriptor_table_ptr);
    idt_descriptor->set_size(sizeof(Interrupts::InterruptDescriptorTable) - 1);

    Interrupts::PIC::PIC_remap(0x28, 0x20);

    asm volatile (
        "lidt %0"
        :: "m" (idt_descriptor) :
    );

    vga.write_string("Kernel done\n", VGA::BG_COLOR::BG_BLACK, VGA::FG_COLOR::GREEN, VGA::BLINK::FALSE);
}
