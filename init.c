#include "hifive1b.h"
#include "interrupts.h"

void setup() __attribute((weak));
void loop() __attribute((weak));

void __use_hfrosc()
{
    // Should be default chip values
    PRCI.hfrosc = (4 << HFROSC_DIV_SHIFT) | (16 << HFROSC_TRIM_SHIFT) | HFROSC_ENABLE;

    // Wait for clock to be available
    while ((PRCI.hfrosc & HFROSC_READY) == 0) asm("");
    
    // Disable PLL, to run from hfrosc while we configure
    PRCI.pllcfg &= ~PLL_SEL;
}

void __init_pll()
{
    // Turn off low frequency ring osc, unnecessary
    AON.lfrosccfg &= ~LFROSC_ENABLE;

    QSPI0.sckdiv = 8;

    __use_hfrosc();
 
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

    // Wait 100 us to stabilize
    uint64_t now = CLINT.mtime;
    while (CLINT.mtime < now + 4) asm("");

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

uint32_t __measure_cpu_freq(int num_ticks)
{
    uint32_t tmp = CLINT.mtime;
    uint32_t start = tmp;
    uint32_t current;

    while (start == tmp) // Wait for an rtc tick
    {
        start = CLINT.mtime;
    }

    uint32_t cycle_start = read_csr(mcycle);

    while (RTC.countlo - start < num_ticks)
    {
        asm("");
    }

    uint32_t mcycle_dt = read_csr(mcycle) - cycle_start;

    return (mcycle_dt / num_ticks) * 32768 + ((mcycle_dt % num_ticks) * 32768) / num_ticks;
}

uint32_t get_cpu_freq()
{
    static uint32_t freq = 0;
    if (!freq)
    {
        __measure_cpu_freq(1);
        freq = __measure_cpu_freq(10);
    }
    return freq;
}

void __systick()
{
    // TODO: add some kind of scheduling here
    CLINT.mtime = 0;
}

void __init()
{
    disable_interrupts();
    __init_data_and_bss();
    __init_pll();

    extern void __init_interrupts();
    __init_interrupts();

    CLINT.mtimecmp = 32768;

    //timer_set_handler(__systick);

    //enable_timer_interrupts();
    enable_interrupts();

    if (setup)
    {
        setup();
    }

    while (loop)
    {
        loop();
    }

    while (1)
    {
        asm("wfi");
    }
}

