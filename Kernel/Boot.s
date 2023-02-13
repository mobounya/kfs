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

/* Initializing a 16 KiB memory area we will use as a stack */
.section .bss
.align 16
stack_bottom:
.skip 1024 * 16
stack_top:

/* Initalize a 40 kb memory area for 10 paging structures */
.section .page_tables, "aw", @nobits
.align 4096
page_tables_base_ptr:
.skip (4096 * 10)

.section .text
.global _start

.extern multiboot_info_ptr
.type multiboot_info_ptr, @object

_start:
    cli
    lgdt [GDT_descriptor]
    mov $0x10, %esp
    mov %esp, %ds
    mov $0x18, %esp
    mov %esp, %ss
    ljmp $0x08, $.call_kernel

.call_kernel:
    movl %ebx, multiboot_info_ptr
    mov $stack_top, %esp # esp now will point to the top of the stack.
    push $page_tables_base_ptr
    call kernel_main
1:	hlt
	jmp 1b

GDT_start:
    GDT_null:
        .8byte 0x0

    kernel_code_descriptor:
        # first 2 bytes of the limit
        .2byte 0xffff

        # first 3 bytes of base address
        .2byte 0x00
        .byte 0x0

        # access/type byte
        .byte 0b10011010

        # 4 bit flag and last 4 bits of limit
        .byte 0b11001111

        # last 1 byte of base address
        .byte 0x0

    kernel_data_descriptor:
        # first 2 bytes of the limit
        .2byte 0xffff

        # first 3 bytes of base address
        .2byte 0x0
        .byte 0x0

        # access/type byte
        .byte 0b10010010

        # 4 bit flag and last 4 bits of limit
        .byte 0b11001111

        # last 1 byte of base address
        .byte 0x0
    
    kernel_stack_descriptor:
        # first 2 bytes of the limit
        .2byte 0xffff

        # first 3 bytes of base address
        .2byte 0x0
        .byte 0x0

        # access/type byte
        .byte 0b10010010

        # 4 bit flag and last 4 bits of limit
        .byte 0b11001111

        # last 1 byte of base address
        .byte 0x0
    
    user_code_descriptor:
        # first 2 bytes of the limit
        .2byte 0xffff

        # first 3 bytes of base address
        .2byte 0x00
        .byte 0x0

        # access/type byte
        .byte 0b11111110

        # 4 bit flag and last 4 bits of limit
        .byte 0b11001111

        # last 1 byte of base address
        .byte 0x0
    
    user_data_descriptor:
        # first 2 bytes of the limit
        .2byte 0xffff

        # first 3 bytes of base address
        .2byte 0x00
        .byte 0x0

        # access/type byte
        .byte 0b11110010

        # 4 bit flag and last 4 bits of limit
        .byte 0b11001111

        # last 1 byte of base address
        .byte 0x0
    
    user_stack_descriptor:
        # first 2 bytes of the limit
        .2byte 0xffff

        # first 3 bytes of base address
        .2byte 0x0
        .byte 0x0

        # access/type byte
        .byte 0b11110010

        # 4 bit flag and last 4 bits of limit
        .byte 0b11001111

        # last 1 byte of base address
        .byte 0x0
GDT_end:

GDT_descriptor:
    .2byte  GDT_end - GDT_start - 1
    .4byte  GDT_start
