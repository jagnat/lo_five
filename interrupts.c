#include "interrupts.h"
#include "hifive1b.h"

static interrupt_handler plic_handlers[PLIC_NUM_INTERRUPTS];
static interrupt_handler timer_handler;


void timer_set_handler(interrupt_handler proc)
{
    timer_handler = proc;
}

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

    for (int i = 0; i < PLIC_NUM_INTERRUPTS; i++)
    {
        plic_handlers[i] = 0;
    }

    timer_handler = 0;

    disable_external_interrupts();
    disable_timer_interrupts();
    disable_software_interrupts();
}

// Blink a red LED to signify hardfault
void __hardfault()
{
    GPIO.output_en = BIT(22);
    while (1)
    {
        GPIO.output_val = 0;
        CLINT.mtime = 0;
        while (CLINT.mtime < 4096) asm("");
        GPIO.output_val = BIT(22);
        CLINT.mtime = 0;
        while (CLINT.mtime < 4096) asm("");
    }
}

void __irq_handler(int cause)
{
    int exception_code = cause & MCAUSE_CODE_MASK;
    uint32_t ext_irq = 0;

    if (cause & MCAUSE_INT) // Interrupt
    {
        switch(exception_code)
        {
            case IRQ_M_SOFT:
                break;
            case IRQ_M_TIMER:
                if (timer_handler)
                    timer_handler();
                break;
            case IRQ_M_EXT:
                ext_irq = PLIC.claim_complete;
                if (ext_irq > 0 && plic_handlers[ext_irq - 1])
                    plic_handlers[ext_irq - 1]();
                PLIC.claim_complete = ext_irq;
                break;
        }
    }
    else // Hw exception or environment call
    {
        switch(exception_code)
        {
            // Environment call from either user/machine mode
            case EXC_ECALL_U:
            case EXC_ECALL_M:
                break;
            default: // Unhandled hw exceptions
                __hardfault();
                break;
        }
    }
}

