SECTIONS
{
	.multiboot_header :
	{
		boot.o (.multiboot_header)
	}

	.text :
	{
		boot.o (.text)
		not_libc.o (.text)
		vga_interface.o (.text)
		kernel.o (.text)
	}

	.rodata :
	{
		boot.o (.rodata)
		not_libc.o (.rodata)
		vga_interface.o (.rodata)
		kernel.o (.rodata)
	}

	.data :
	{
		boot.o (.data)
		not_libc.o (.data)
		vga_interface.o (.data)
		kernel.o (.data)
	}

	.bss :
	{
		boot.o (.bss)
		not_libc.o (.bss)
		vga_interface.o (.bss)
		kernel.o (.bss)
	}
}
