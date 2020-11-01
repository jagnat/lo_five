#include "hifive1b.h"
#include "task.h"
#include "interrupt.h"

void task2();
void task1();

NEW_TASK(blinky2, task2, 0, 100, 0);

sem_t lock = {1, 0};

void rtc_irq()
{
    RTC.countlo = 0;
    sem_signal_from_isr(&lock);
}

void setup()
{
    GPIO.output_en = RED_LED_PIN | GREEN_LED_PIN | BLUE_LED_PIN;
    GPIO.output_xor = RED_LED_PIN | GREEN_LED_PIN | BLUE_LED_PIN;
    GPIO.output_val = 0;

    RTC.countlo = 0;
    RTC.counthi = 0;
    RTC.cfg = RTC_ENALWAYS;
    RTC.cmp0 = 32768;
    plic_set_handler(PLIC_INT_RTC, rtc_irq);
    plic_enable(PLIC_INT_RTC, 7);
}

void task2()
{
    while (1)
    {
        sem_wait(&lock);
        GPIO.output_val |= BLUE_LED_PIN;
        for (int i = 0; i < 40000; i++) asm("");
        GPIO.output_val &= ~BLUE_LED_PIN;
        for (int i = 0; i < 40000; i++) asm("");
    }
}

