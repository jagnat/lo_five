#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include "hifive1b.h"

static inline void enable_interrupts() { set_csr(mstatus, MSTATUS_MIE); }
static inline void disable_interrupts() { clear_csr(mstatus, MSTATUS_MIE); }

typedef void (*interrupt_handler)(void);

void plic_clear();
void plic_set_handler(int interrupt, interrupt_handler proc);
void plic_enable (int interrupt, int priority);
void plic_disable(int interrupt);

#endif
