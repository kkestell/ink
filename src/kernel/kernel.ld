KERNEL_PHYS_START = 1M;
KERNEL_STACK_SIZE = 0x4000;

ENTRY (_start)

SECTIONS
{
	. = KERNEL_PHYS_START;
	kernel_start = .;

	.text : ALIGN (4K)
	{
		*(.text*)
	}

	.rodata : ALIGN (4K)
	{
		*(.rodata*)
	}

	.data : ALIGN (4K)
	{
		*(.data*)
	}

	.bss : ALIGN (4K)
	{
		*(COMMON)
		*(.bss*)

    . = ALIGN (16);
    stack_bottom = .;
    . += KERNEL_STACK_SIZE;
    stack_top = .;
	}

	kernel_end = .;
}