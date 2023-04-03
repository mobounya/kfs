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

	.interrupt_descriptor_table :
	{
		*(.interrupt_descriptor_table)
	}

	.reserved_kernel_memory :
	{
		*(.reserved_kernel_memory)
	}
}
