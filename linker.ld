SECTIONS
{
	.multiboot_header :
	{
		boot.o (.multiboot_header)
	}

	.text :
	{
		boot.o (.text)
		kernel.o (.text)
	}

	.rodata :
	{
		boot.o (.rodata)
		kernel.o (.rodata)
	}

	.data :
	{
		boot.o (.text)
		kernel.o (.data)
	}

	.bss :
	{
		boot.o (.bss)
		kernel.o (.bss)
	}
}
