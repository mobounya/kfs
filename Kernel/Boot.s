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
.skip (8 * 255) # 255 entry 8 bytes each
idt_descriptor_ptr:
.skip 6

.section .text
.global _start

.extern multiboot_info_ptr
.type multiboot_info_ptr, @object

_start:
    push $stack_top
    call setup_gdt
    lgdt [GDT_descriptor]
    # ltr 0x38 # let's not load the tss segment for now.
    mov $0x10, %esp
    mov %esp, %ds
    mov $0x18, %esp
    mov %esp, %ss
    ljmp $0x08, $.call_kernel

.call_kernel:
    movl %ebx, multiboot_info_ptr
    mov $stack_top, %esp # esp now will point to the top of the stack.
    push $kernel_page_tables
    push $user_page_tables
    push $interrupt_descriptor_table_ptr
    push $idt_descriptor_ptr
    call kernel_main
1:	hlt
	jmp 1b

GDT_descriptor:
    .2byte  63
    .4byte  0x00000800
