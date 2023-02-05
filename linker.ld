SECTIONS
{
	.multiboot_header :
	{
		*(.multiboot_header)
	}

	.text :
	{
		*(.text)
	}

	.rodata :
	{
		*(.rodata)
	}

	.data :
	{
		*(.data)
	}

	.bss :
	{
		*(.bss)
	}
}
