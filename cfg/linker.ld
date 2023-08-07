ENTRY(_start)

/* Below this address is reserved/used by grub, bios, ... */
KERNEL_LMA = 1M;
/* This is the start address of the last possible pml4 entry */
KERNEL_VMA = 0xFFFFFF8000000000;

SECTIONS
{
	. = KERNEL_LMA + KERNEL_VMA;

	PROVIDE(kernel_start_ = ABSOLUTE(.));

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VMA)
	{
		KEEP(*(.multiboot_header))
		*(.text)
	}

	/* Read-only data. */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VMA)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VMA)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VMA)
	{
		*(COMMON)
		*(.bss)
	}

	PROVIDE(kernel_end_ = ABSOLUTE(.));
}
