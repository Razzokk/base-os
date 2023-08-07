ENTRY(_start)

SECTIONS
{
	. = 1M;

	PROVIDE(kernel_start_ = ABSOLUTE(.));

	.text ALIGN(4K) :
	{
		*(.multiboot_header)
		*(.text)
	}
 
	/* Read-only data. */
	.rodata ALIGN(4K) :
	{
		*(.rodata)
	}
 
	/* Read-write data (initialized) */
	.data ALIGN(4K) :
	{
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) :
	{
		*(COMMON)
		*(.bss)
	}

	PROVIDE(kernel_end_ = ABSOLUTE(.));
}
