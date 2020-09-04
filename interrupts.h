#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include "hifive1b.h"

static inline void enable_interrupts() { set_csr(mstatus, MSTATUS_MIE); }
static inline void disable_interrupts() { clear_csr(mstatus, MSTATUS_MIE); }

static inline void enable_external_interrupts() { set_csr(mie, MIP_MEIP); }
static inline void disable_external_interrupts() { clear_csr(mie, MIP_MEIP); }
static inline void enable_timer_interrupts() { set_csr(mie, MIP_MTIP); }
static inline void disable_timer_interrupts() { clear_csr(mie, MIP_MTIP); }
static inline void enable_software_interrupts() { set_csr(mie, MIP_MSIP); }
static inline void disable_software_interrupts() { clear_csr(mie, MIP_MSIP); }

typedef void (*interrupt_handler)(void);

void timer_set_handler(interrupt_handler proc);

void plic_clear();
void plic_set_handler(int interrupt, interrupt_handler proc);
void plic_enable (int interrupt, int priority);
void plic_disable(int interrupt);

extern const int __in_irq;

#endif
