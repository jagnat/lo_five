#include "hifive1b.h"
#include "interrupts.h"

void __init_pll()
{
    // Enable hfrosc temporarily
    // divider, trim, enable
    // Should be default chip values
    PRCI.hfrosc = (4 << HFROSC_DIV_SHIFT) | (16 << HFROSC_TRIM_SHIFT) | HFROSC_ENABLE;

    // Wait for clock to be available
    while ((PRCI.hfrosc & HFROSC_READY) == 0) asm("");

    // Turn off low frequency ring osc, unnecessary
    AON.lfrosccfg &= ~LFROSC_ENABLE;

    // Disable PLL, to run from hfrosc while we configure
    PRCI.pllcfg &= ~(1 << 16); // pllsel
    
    // Turn on hfxosc
    PRCI.hfxosc = HFXOSC_ENABLE;
    while ((PRCI.hfxosc & HFXOSC_READY) == 0) asm("");

    // Bypass PLL while configuring
    PRCI.pllcfg |= PLL_BYPASS;

    // Configure pll values
    uint32_t pll = 0;
    pll |= (1 << PLL_R_SHIFT); // Set R = 2 (value + 1)
    pll |= (31 << PLL_F_SHIFT); // Set F = 64 ((value + 1) * 2)
    pll |= (1 << PLL_Q_SHIFT); // Set q = 2 (value + 1, 0 not supported)
    pll |= PLL_BYPASS; // Enable bypass while setting values

    // Set no final division
    PRCI.plloutdiv = PLL_DIVIDE_BY_1;

    PRCI.pllcfg = pll; // set pll
    PRCI.pllcfg &= ~PLL_BYPASS; // turn off bypass

    // Wait some time
    // TODO: Tune this maybe using rt clock
    for (int i = 0; i < 1000000; i++) asm("");

    // Now safe to check pll lock
    while ((PRCI.pllcfg & PLL_LOCK) == 0) asm("");
    PRCI.pllcfg |= PLL_SEL; // Turn on pllsel
}


void __init_data_and_bss()
{
    extern uint32_t __DATA_FLASH;
    extern uint32_t __DATA_RAM;
    extern uint32_t __DATA_RAM_END;
    extern uint32_t __BSS_ADDR;
    extern uint32_t __BSS_END;

    register uint32_t *data_flash = &__DATA_FLASH;
    register uint32_t *data_ram = &__DATA_RAM;

    while (data_ram != &__DATA_RAM_END)
        *(data_ram++) = *(data_flash++);

    register uint32_t *bss = &__BSS_ADDR;
    while (bss != &__BSS_END)
        *(bss++) = 0;
}

void time_handler()
{
    CLINT.mtime = 0;
    GPIO.output_val = ~BIT(19);
    for (int i = 0; i < 30000; i++)
        asm("");
    GPIO.output_val = BIT(19);
}

void __init()
{
    disable_interrupts();
    __init_data_and_bss();
    __init_pll();

    extern void __init_interrupts();
    __init_interrupts();

    CLINT.mtimecmp = 32768;

    timer_set_handler(time_handler);

    enable_timer_interrupts();
    enable_interrupts();

    GPIO.output_en = BIT(19);
    GPIO.output_val = BIT(19);

    while (1)
    {
        asm("wfi");
    }
}



