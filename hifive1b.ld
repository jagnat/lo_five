
ENTRY(__entry)

MEMORY
{
	ram : org = 0x80000000, len = 4k
	flash : org = 0x20000000, len = 0x400000
}

SECTIONS
{
	.text :
	{
        . = 0x00000000;
        *(.entry)
        . = ALIGN(4);
        *(.text)
	} > flash

	.rodata :
	{
        *(.rodata)
	} > flash

    .data :
    {
        *(.data)
    } > ram AT > flash

	.bss :
	{
        *(.bss)
	} > ram
}

GPIO = 0x10012000;
