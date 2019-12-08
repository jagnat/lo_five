
#include "hifive1b.h"
#include "csr_encoding.h"

void __pll_init()
{
    // Enable hfrosc temporarily
    // divider, trim, enable
    // Should be default chip values
    PRCI.hfrosc = (4 << HFROSC_DIV_SHIFT) | (16 << HFROSC_TRIM_SHIFT) | HFROSC_ENABLE;

    // Wait for clock to be available
    while ((PRCI.hfrosc & HFROSC_READY) == 0) asm("");

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

extern void __irq_proc();

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

void __init()
{
    __init_data_and_bss();
    write_csr(mtvec, &__irq_proc);
    __pll_init();

    // Set PWM
    PWM1.cfg = (1 << 12) | (7 << 0); // Enable always, scale bottom x bits
    PWM1.count = 0;
    PWM1.cmp0 = 0;
    PWM1.cmp3 = 400;

    GPIO.input_en = GPIO.input_en & ~(BIT(22));
    GPIO.iof_en = BIT(22);
    GPIO.iof_sel = BIT(22);

    while (1)
    {
        asm("wfi");
    }
}

void __irq_handler(int cause)
{
    asm("");
}

