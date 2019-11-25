
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
PRCI = 0x10008000;

PWM0 = 0x10015000;
PWM1 = 0x10025000;
PWM2 = 0x10035000;
