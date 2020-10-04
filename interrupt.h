
#pragma once

#include "hifive1b.h"

static inline int enable_interrupts()
{
    unsigned prev = read_csr(mstatus);
    set_csr(mstatus, MSTATUS_MIE);
    return prev & MSTATUS_MIE;
}
static inline int disable_interrupts()
{
    unsigned prev = read_csr(mstatus);
    clear_csr(mstatus, MSTATUS_MIE);
    return prev & MSTATUS_MIE;
}

static inline void restore_interrupts(int prev)
{
    if (prev)
    {
        set_csr(mstatus, MSTATUS_MIE);
    }
    else
    {
        clear_csr(mstatus, MSTATUS_MIE);
    }
}

static inline void enable_external_interrupts() { set_csr(mie, MIP_MEIP); }
static inline void disable_external_interrupts() { clear_csr(mie, MIP_MEIP); }
static inline void enable_timer_interrupts() { set_csr(mie, MIP_MTIP); }
static inline void disable_timer_interrupts() { clear_csr(mie, MIP_MTIP); }
static inline void enable_software_interrupts() { set_csr(mie, MIP_MSIP); }
static inline void disable_software_interrupts() { clear_csr(mie, MIP_MSIP); }

typedef void (*interrupt_handler)(void);

void timer_set_handler(interrupt_handler proc);
void ecall_set_handler(interrupt_handler proc);

void plic_clear();
void plic_set_handler(int interrupt, interrupt_handler proc);
void plic_enable (int interrupt, int priority);
void plic_disable(int interrupt);

int in_irq();

