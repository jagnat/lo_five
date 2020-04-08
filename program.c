#include "hifive1b.h"
#include "task.h"
#include "interrupts.h"

void task2();

NEW_TASK(blinky, task2, 0, 100, 0);

sem_t lock = {1, 0};

void setup()
{
    GPIO.output_en = BIT(21) | BIT(19);
    GPIO.output_xor = BIT(21) | BIT(19);
    GPIO.output_val = 0;

    RTC.countlo = 0;
    RTC.counthi = 0;
    RTC.cfg = RTC_ENALWAYS;
}

void loop()
{
    sem_wait(&lock);
    GPIO.output_val |= BIT(21);
    for (int i = 0; i < 4000000; i++) asm("");
    GPIO.output_val &= ~BIT(21);
    for (int i = 0; i < 4000000; i++) asm("");
    sem_signal(&lock);
}

void task2()
{
    while (1)
    {
        sem_wait(&lock);
        GPIO.output_val |= BIT(19);
        for (int i = 0; i < 4000000; i++) asm("");
        GPIO.output_val &= ~BIT(19);
        for (int i = 0; i < 4000000; i++) asm("");
        sem_signal(&lock);
    }
}
