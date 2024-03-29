#include <stdint.h>
#include <stddef.h>
#include <Kernel/Memory/KernelVirtualMemoryManager.hpp>
#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/Memory/PhysicalMemoryManager.hpp>
#include <Kernel/Multiboot/Multiboot.hpp>
#include <Kernel/Memory/MemoryRegion.hpp>
#include <Kernel/Memory/PagingStructureEntry.hpp>
#include <Kernel/Memory/MemoryPage.hpp>
#include <Kernel/Memory/KernelVirtualMemoryManager.hpp>
#include <Kernel/Memory/UserVirtualMemoryManager.hpp>
#include <Kernel/Memory/VirtualAddress.hpp>
#include <Kernel/Interrupts/IDT.hpp>
#include <Kernel/Interrupts/PIC.hpp>
#include <Kernel/GDT/GDT.hpp>
#include <Kernel/GDT/TSS.hpp>
#include <Kernel/CPU/CPU.hpp>
#include <Kernel/Interrupts/PIC.hpp>
#include <Kernel/Memory/QuickDirtyMalloc.hpp>
#include <Kernel/Display/Screen.hpp>
#include <Kernel/Debug/Debug.hpp>
#include <Kernel/Signals/Signals.hpp>
#include <Kernel/Devices/Keyboard.hpp>

#include <string.hpp>
#include <unordered_map.hpp>
#include <cstring.h>

extern "C" {
    multiboot_info  *multiboot_info_ptr;
    void            load_idt(void);
    void            NMI_interrupt_as(void);
    void            DB_fault_as(void);
    void            UD_fault_as(void);
    void            BP_trap_as(void);
    void            NM_fault_as(void);
    void            DF_abort_as(void);
    void            TS_fault_as(void);
    void            NP_fault_as(void);
    void            SS_fault_as(void);
    void            GP_fault_as(void);
    void            PF_fault_as(void);
    void            MF_fault_as(void);
    void            AC_fault_as(void);
    void            MC_fault_as(void);
    void            XM_fault_as(void);
    void            keyboard_handler_as(void);
    void            default_handler_as(void);
    void            signal_handler_1_as(void);
    void            signal_handler_2_as(void);
}

void print_multiboot_info()
{
    Screen cout;

    if (multiboot_info_ptr->flags & MULTIBOOT_BOOT_LOADER_NAME) {
        cout << "Loaded by:\n -" << (char *)(multiboot_info_ptr->boot_loader_name) << "\n";
    }
    if (multiboot_info_ptr->flags & MULTIBOOT_MEMORY_INFO)
        cout << "Memory info available" << "\n";
    if (multiboot_info_ptr->flags & MULTIBOOT_BOOT_DEVICE)
        cout << "Boot device info available" << "\n";
    if (multiboot_info_ptr->flags & MULTIBOOT_CMDLINE)
        cout << "Command line info available" << "\n";
    if (multiboot_info_ptr->flags & MULTIBOOT_MODS)
        cout << "Boot modules info available" << "\n";
    if (multiboot_info_ptr->flags & MULTIBOOT_SYMS)
        cout << "SYMS info available" << "\n";
    if (multiboot_info_ptr->flags & MULTIBOOT_MEMORY_MAP)
        cout << "Memory map info available" << "\n";
    if (multiboot_info_ptr->flags & MULTIBOOT_DRIVE)
        cout << "Drives info available" << "\n";
    if (multiboot_info_ptr->flags & MULTIBOOT_CONFIG_TABLE)
        cout << "ROM configuration info available" << "\n";
    if (multiboot_info_ptr->flags & MULTIBOOT_APM_TABLE)
        cout << "APM info available" << "\n";
    if (multiboot_info_ptr->flags & MULTIBOOT_VBE_TABLE)
        cout << "VBE info available" << "\n";
    if (multiboot_info_ptr->flags & MULTIBOOT_FRAMEBUFFER_TABLE)
        cout << "VBE info available" << "\n";
}

/*
    grub_multiboot_info_ptr is the pointer given to us by grub in the ebx register.
    multiboot_info_section_ptr is a pointer to a location in memory enough to fit the structure.
*/
extern "C" void copy_multiboot_info_structure(void *grub_multiboot_info_ptr, void *multiboot_info_section_ptr)
{
    // TODO: you need to save the content of any pointer in the multiboot info structure in order to use it.
    // for now we will save the content of mmap_addr and boot_loader_name since that's the only two we using currently.

    size_t section_offset = sizeof(multiboot_info);
    multiboot_info  *multi_info_ptr = (multiboot_info *)grub_multiboot_info_ptr;

    // save the data of mmap_addr address.
    void *mmap_addr = memcpy(((uint8_t *)multiboot_info_section_ptr) + section_offset, multi_info_ptr->mmap_addr, multi_info_ptr->mmap_length);
    multi_info_ptr->mmap_addr = (multiboot_mmap *)mmap_addr;
    section_offset += multi_info_ptr->mmap_length;

    // save the data of boot_loader_name address.
    void *boot_loader_name_addr = memcpy(((uint8_t *)multiboot_info_section_ptr) + section_offset, (void *)(multi_info_ptr->boot_loader_name), strlen((char *)multi_info_ptr->boot_loader_name));
    multi_info_ptr->boot_loader_name = (uint32_t)boot_loader_name_addr;
    section_offset += strlen((char *)multi_info_ptr->boot_loader_name) + 1;

    // save the entire structure.
    memcpy(multiboot_info_section_ptr, multi_info_ptr, sizeof(multiboot_info));
}

extern "C" void setup_gdt(void *stack_ptr)
{
    Kernel::GDT_Table   *gdt_table_ptr = (Kernel::GDT_Table *)0x1000;
    // Store the tss right after the GDT.
    // Kernel::TSS         *tss_ptr = (Kernel::TSS *)(((Kernel::GDT_Table *)0x1000) + 1);
    Kernel::GDT_Table gdt_table;

    size_t  gdt_size = 0;
    uint8_t access_byte = PRESENT_SEGMENT | CODE_DATA_SEGMENT | EXECUTABLE_SEGMENT | ALLOW_READ_ACCESS;
    uint8_t flags = GRANUALITY_FLAG | SEGMENT_32BIT_FLAG;

    memset(gdt_table_ptr, 0x00, sizeof(Kernel::GDT_Table));

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
    // access_byte = PRESENT_SEGMENT | DPL_USER | CODE_DATA_SEGMENT | EXECUTABLE_SEGMENT | ALLOW_READ_ACCESS;
    // gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor(0x0, 0xfffff, access_byte, flags), gdt_size++);

    // // Segment descriptor for user data.
    // access_byte = PRESENT_SEGMENT | DPL_USER | CODE_DATA_SEGMENT | ALLOW_WRITE_ACCESS;
    // gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor(0x0, 0xfffff, access_byte, flags), gdt_size++);

    // // Segment descriptor for user stack.
    // access_byte = PRESENT_SEGMENT | DPL_USER | CODE_DATA_SEGMENT | ALLOW_WRITE_ACCESS;
    // gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor(0x0, 0xfffff, access_byte, flags), gdt_size++);


    // Setup the tss.
    // memset(tss_ptr, 0x0, sizeof(Kernel::TSS));
    // tss_ptr->set_SS0(0x10);
    (void)stack_ptr;
    // tss_ptr->set_ESP0((uint32_t)stack_ptr);
    // tss_ptr->set_IOPB(sizeof(Kernel::TSS) - 4);

    // System segment descriptor for TSS.
    // access_byte = 0x89;
    // flags = 0x8;
    // https://wiki.osdev.org/Task_State_Segment
    // IOPB may get the value sizeof(TSS) (which is 104) if you don't plan to use this io-bitmap further (according to mystran in http://forum.osdev.org/viewtopic.php?t=13678)
    // gdt_table.insert_new_entry(Kernel::GDT::create_GDT_Descriptor((uint32_t)tss_ptr, sizeof(Kernel::TSS), access_byte, flags), gdt_size++);

    *gdt_table_ptr = gdt_table;
}

extern "C" void kernel_main(void *kernel_page_tables, void *interrupt_descriptor_table_ptr, void *idt_descriptor_ptr)
{
    Screen cout;
    Memory::PhysicalMemoryManager           &memory_manager = Memory::PhysicalMemoryManager::instantiate();
    Memory::KernelVirtualMemoryManager      kernel_vm(kernel_page_tables);
    uint32_t                                mmap_length = multiboot_info_ptr->mmap_length;
    multiboot_mmap                          *mmap_addr = multiboot_info_ptr->mmap_addr;
    uint32_t                                mmap_structure_size;
    Interrupts::InterruptDescriptorTable    *interrupt_descriptor_table = (Interrupts::InterruptDescriptorTable *)interrupt_descriptor_table_ptr;
    Interrupts::IDTDescriptor               *idt_descriptor = (Interrupts::IDTDescriptor *)idt_descriptor_ptr;
    Signals                                 signals(interrupt_descriptor_table);

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
            - 0x100000 --> 0x300000  : 3 Mib, Kernel image (?? I don't know the size of the kernel image).
            - 0x300000 --> 0x800000  : 4 Mib, Available for allocation for the Kernel.
            - 0x800000 --> 0x1000000 : 8 Mib, Available for allocation for user space programs. 
    */

    // Identity map the first 1 Mib (Mebibyte), 0x0 --> 0x100000
    kernel_vm.identity_map_memory(VirtualAddress((void *)0x0), VirtualAddress((void *)0x100000));

    // Identity map the Kernel image, 0x100000 --> 0x400000
    kernel_vm.identity_map_memory(VirtualAddress((void *)0x100000), VirtualAddress((void *)0x300000));

    kernel_vm.load_page_directory();

    // Disable first page so de-refrencing a NULL ptr would not work.
    kernel_vm.disable_page(VirtualAddress((void *)0x0), PAGE_SIZE);

    Interrupts::PIC::PIC_remap(0x20, 0x28);

    Interrupts::PIC::mask_all();

    Interrupts::GateDescriptor descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)DB_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 1);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)NMI_interrupt_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 2);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)BP_trap_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 3);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)UD_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 6);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)NM_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 7);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)DF_abort_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 8);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)TS_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 10);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)NP_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 11);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)SS_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 12);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)GP_fault_as, 0x8, Interrupts::GateType::TrapGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 13);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)PF_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 14);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)MF_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 16);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)AC_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 17);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)MC_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 18);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)XM_fault_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_intel_interrupt_handler(descriptor, 19);

    descriptor = Interrupts::GateDescriptor::create_gate_descriptor((uint32_t)keyboard_handler_as, 0x8, Interrupts::GateType::InterruptGate32, 0x0, true);
    interrupt_descriptor_table->insert_irq_handler(descriptor, 1);

    idt_descriptor->set_address((uint32_t)interrupt_descriptor_table_ptr);
    idt_descriptor->set_size(sizeof(Interrupts::InterruptDescriptorTable) - 1);

    // Keybaord IRQ
    Interrupts::PIC::IRQ_clear_mask(1);
    // cascade IRQ
    // Interrupts::PIC::IRQ_clear_mask(2);

    load_idt();

    Interrupts::PIC::enable();

    memory_manager.enable_paging();

    signals.register_signal(SIGNAL_1, signal_handler_1_as);
    signals.register_signal(SIGNAL_2, signal_handler_2_as);

    Keyboard &keybrd = Keyboard::instantiate();

    // F1
    keybrd.map_key(0x3B, [](){
        Signals::send_signal(SIGNAL_1);
    });

    // F2
    keybrd.map_key(0x3C, [](){
        Signals::send_signal(SIGNAL_2);
    });
}
