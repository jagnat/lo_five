#include "hifive1b.h"
#include "task.h"
#include "interrupt.h"

sem_t lock = {0};

void rtc_irq()
{
    RTC.countlo = 0;
    RTC.counthi = 0;
    sem_signal_from_isr(&lock);
}

void setup()
{
    GPIO.output_en = BLUE_LED_PIN;
    GPIO.output_xor = BLUE_LED_PIN;
    GPIO.output_val = 0;
    
    RTC.countlo = 0;
    RTC.counthi = 0;
    RTC.cfg = RTC_ENALWAYS;
    RTC.cmp0 = 32768;
    plic_set_handler(PLIC_INT_RTC, rtc_irq);
    plic_enable(PLIC_INT_RTC, 7);
}

void loop()
{
    while (1)
    {
        sem_wait(&lock);
        GPIO.output_val ^= BLUE_LED_PIN;
    }
}

