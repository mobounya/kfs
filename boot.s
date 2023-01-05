.set MAGIC, 0x1BADB002                             # magic number identifying a multiboot header.
.set ALIGN_PAGE_BOUNDARIES, 1                      # all boot modules loaded along with the operating system must be aligned on page (4KB) boundaries.
.set MEMORY_INFO, 2                                # available information on the available information.
.set FLAGS, (ALIGN_PAGE_BOUNDARIES | MEMORY_INFO)  # flags to specify which information we need from the bootloader.
.set CHECKSUM, -(MAGIC + FLAGS)                    # If you add this checksum to MAGIC and FLAGS you need to get "0" to prove we are multiboot.

/* Section for the multiboot header */
.section multiboot_header
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Initializing a 16 KiB memory area we will use as a stack */
.section .bss
.align 16
stack_bottom:
.skip 16384, 0    # skipping 16 KiB and fill it with value 0
stack_top:

.section .text
.global _start

_start:
    mov $stack_top, %esp # esp now will point to the top of the stack.
    call kernel_main
    cli
1:	hlt
	jmp 1b
