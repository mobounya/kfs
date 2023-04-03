.set MAGIC, 0x1BADB002                             # magic number identifying a multiboot header.
.set ALIGN_PAGE_BOUNDARIES, 1                      # all boot modules loaded along with the operating system must be aligned on page (4KB) boundaries.
.set MEMORY_INFO, 2                                # available information on the available information.
.set FLAGS, (ALIGN_PAGE_BOUNDARIES | MEMORY_INFO)  # flags to specify which information we need from the bootloader.
.set CHECKSUM, -(MAGIC + FLAGS)                    # If you add this checksum to MAGIC and FLAGS you need to get "0" to prove we are multiboot.

/* Section for the multiboot header */
.section .multiboot_header
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Initializing a 1 MiB memory area we will use as a stack */
.section .bss
.align 16
stack_bottom:
.skip 1079296
stack_top:

/*
    Initalize 1 Page directory and 4 Page tables for the kernel
    and
    1 Page directory and 4 Page tables for user space (just for testing)
*/

.section .page_tables, "aw", @nobits
.align 4096
kernel_page_tables:
.skip (4096 * 5)
user_page_tables:
.skip (4096 * 5)

.section .interrupt_descriptor_table, "aw", @nobits
.align 8
interrupt_descriptor_table_ptr:
.skip (8 * 256) # 256 entry 8 bytes each
idt_descriptor_ptr:
.skip 6

.section .reserved_kernel_memory, "aw", @nobits
reserved_kernel_memory_start:
.skip 1024 * 10 # 10 KiB
reserved_kernel_memory_end:


.section .text
.global _start
.global load_idt
.global send_signal
.global call_kernel

.global DB_fault_as
.global NMI_interrupt_as
.global UD_fault_as
.global BP_trap_as
.global NM_fault_as
.global DF_abort_as
.global TS_fault_as
.global NP_fault_as
.global SS_fault_as
.global GP_fault_as
.global PF_fault_as
.global MF_fault_as
.global AC_fault_as
.global MC_fault_as
.global XM_fault_as

.global keyboard_handler_as
.global default_handler_as

.extern multiboot_info_ptr
.type multiboot_info_ptr, @object

.extern kernel_memory_ptr
.type kernel_memory_ptr, @object

_start:
    cli
    mov $reserved_kernel_memory_start, %esp
    mov %esp, kernel_memory_ptr
    mov $stack_top, %esp
    push $stack_top
    call setup_gdt
    lgdt [GDT_descriptor]
    # ltr 0x38 # let's not load the tss segment for now.
    ljmp $0x08, $call_kernel

call_kernel:
    mov $0x10, %esp
    mov %esp, %ds
    mov $0x18, %esp
    mov %esp, %ss
    movl %ebx, multiboot_info_ptr
    mov $stack_top, %esp # esp now will point to the top of the stack.
    push $idt_descriptor_ptr
    push $interrupt_descriptor_table_ptr
    push $user_page_tables
    push $kernel_page_tables
    call kernel_main
1:	hlt
	jmp 1b

# Other registers: %eip %eflags %cs %ss %ds %es %fs %gs %cr0 %cr2 %cr3 %cr4 %cr8
# Pushed by the compiler : %ebp %ecx %edx %eax
# General registers: %eax %ebx %ecx %edx %esp %ebp %esi %edi

BP_trap_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call BP_trap
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

DB_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call DB_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

NMI_interrupt_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call NMI_interrupt
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

UD_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call UD_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

NM_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call NM_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

DF_abort_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call DF_abort
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

TS_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call TS_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

NP_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call NP_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

SS_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call SS_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

GP_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call GP_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    add $4, %esp
    iret

PF_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call PF_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

MF_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call MF_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

AC_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call AC_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

MC_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call MC_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

XM_fault_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call XM_fault
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

keyboard_handler_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call keyboard_handler
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax 
    iret

default_handler_as:
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esp
    push %ebp
    push %esi
    push %edi
    call default_handler
    pop %edi
    pop %esi
    pop %ebp
    pop %esp
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    iret

load_idt:
    lidt [idt_descriptor_ptr]
    ret

send_signal:
    int $0x35
    ret

GDT_descriptor:
    .2byte  31
    .4byte  0x00000800
