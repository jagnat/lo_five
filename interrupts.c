#include "interrupts.h"
#include "hifive1b.h"

static 
interrupt_handler plic_handlers[PLIC_NUM_INTERRUPTS];

void plic_clear()
{
    disable_interrupts();
    for (int i = 1; i <= PLIC_NUM_INTERRUPTS; i++)
    {
        PLIC.priority[i] = 0;
    }

    PLIC.enable1 = 0;
    PLIC.enable2 = 0;
    PLIC.threshold = 0;
}

void plic_set_handler(int interrupt, interrupt_handler proc)
{
    if (interrupt < 1 || interrupt > PLIC_NUM_INTERRUPTS)
        return; // TODO: assert
    plic_handlers[interrupt - 1] = proc;
}

void plic_enable (int interrupt, int priority)
{
    if (interrupt < 1 || interrupt > PLIC_NUM_INTERRUPTS)
        return; // TODO: assert
    PLIC.priority[interrupt] = priority;
    if (interrupt >= 32)
        PLIC.enable2 |= (1 << (interrupt % 32));
    else
        PLIC.enable1 |= (1 << interrupt);
}

void plic_disable(int interrupt)
{
    if (interrupt < 1 || interrupt > PLIC_NUM_INTERRUPTS)
        return; // TODO: assert
    if (interrupt >= 32)
        PLIC.enable2 &= ~(1 << (interrupt % 32));
    else
        PLIC.enable1 &= ~(1 << interrupt);
}

void __init_interrupts()
{
    plic_clear();
    extern void __irq_proc();
    write_csr(mtvec, &__irq_proc);

    // turn on external interrupts
    set_csr(mie, MIP_MEIP);
}

void __handle_plic_interrupt()
{
    uint32_t interrupt = PLIC.claim_complete;
    if (interrupt < 1 || interrupt > PLIC_NUM_INTERRUPTS)
    {
        PLIC.claim_complete = interrupt;
        return; // TODO: assert
    }

    plic_handlers[interrupt - 1]();

    PLIC.claim_complete = interrupt;
}

void __irq_handler(int cause)
{
    int exception_code = cause & MCAUSE_CODE_MASK;
    if (cause & MCAUSE_INT) // Interrupt
    {
        switch(exception_code)
        {
            // TODO: add syscall and systick here
            case IRQ_M_SOFT:
            case IRQ_M_TIMER:
                break;
            case IRQ_M_EXT:
                __handle_plic_interrupt();
                break;
        }
    }
    else // Hw exception
    {
        // TODO: handle elsewhere? just trap?
    }
}

