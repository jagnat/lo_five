
ENTRY(__reset)

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
        . = ALIGN(4);
	} > flash

	.rodata :
	{
        *(.rodata)
        . = ALIGN(4);
	} > flash

    .data :
    {
        *(.sdata)
        . = ALIGN(4);
        *(.data)
        . = ALIGN(4);
    } > ram AT > flash

	.bss :
	{
        *(.bss)
        . = ALIGN(4);
	} > ram
}

__DATA_FLASH = LOADADDR(.data);
__DATA_RAM = ADDR(.data);
__DATA_RAM_END = ADDR(.data) + SIZEOF(.data);
__BSS_ADDR = ADDR(.bss);
__BSS_END = ADDR(.bss) + SIZEOF(.bss);

__SP = ORIGIN(ram) + LENGTH(ram);

PLIC = 0x0c000000;
GPIO = 0x10012000;
PRCI = 0x10008000;

PWM0 = 0x10015000;
PWM1 = 0x10025000;
PWM2 = 0x10035000;
