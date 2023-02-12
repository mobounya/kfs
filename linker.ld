ENTRY(_start)

SECTIONS
{
	. = 1M;

	.text :
	{
		*(.multiboot_header)
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

	.page_tables :
	{
		*(.page_tables)
	}
}
