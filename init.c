
#include "hifive1b.h"

void pll_init()
{
    // Enable hfrosc temporarily
    // divider, trim, enable
    // Should be default chip values
    PRCI.hfrosc = (4 << 0) | (16 << 16) | (1 << 30);

    // Wait for clock to be available
    while ((PRCI.hfrosc & (1 << 31)) == 0) asm("");

    // Disable PLL, to run from hfrosc while we configure
    PRCI.pllcfg &= ~(1 << 16); // pllsel
    
    // Turn on hfxosc
    PRCI.hfxosc |= (1 << 30);
    while ((PRCI.hfxosc & (1 << 31)) == 0) asm("");

    // Bypass PLL while configuring
    PRCI.pllcfg |= (1 << 18);

    // Configure pll values
    uint32_t pll = 0;
    pll |= (1 << 0); // Set R = 2 (value + 1)
    pll |= (31 << 4); // Set F = 64 ((value + 1) * 2)
    pll |= (1 << 10); // Set q = 2 (value + 1, 0 not supported)
    pll |= (1 << 18); // Enable bypass while setting values

    // Set no final division
    PRCI.plloutdiv = (1 << 8);

    PRCI.pllcfg = pll; // set pll
    PRCI.pllcfg &= ~(1 << 18); // turn off bypass

    // Wait some time
    for (int i = 0; i < 1000000; i++) asm("");

    // Now safe to check pll lock
    while ((PRCI.pllcfg & (1 << 31)) == 0) asm("");
    PRCI.pllcfg |= (1 << 16); // Turn on pllsel
}

void __init()
{
    pll_init();

    // Set PWM
    PWM1.cfg = (1 << 12) | (15 << 0); // Enable always, scale bottom x bits
    PWM1.count = 0;
    PWM1.cmp0 = 0;
    PWM1.cmp3 = 8000;

    GPIO.input_en = GPIO.input_en & ~(BIT(22));
    GPIO.iof_en = BIT(22);
    GPIO.iof_sel = BIT(22);

    while (1)
    {
        asm("wfi");
        /*
        GPIO.output_val |= BIT(22);
        for (int i = 0; i < 1000000; i++)
          asm("");
        GPIO.output_val &= ~(BIT(22));
        for (int i = 0; i < 1000000; i++)
          asm("");
        */
    }
}
