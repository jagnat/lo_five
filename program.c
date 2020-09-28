#include "hifive1b.h"
#include "task.h"
#include "interrupt.h"

void task2();
void task1();

NEW_TASK(blinky, task1, 0, 100, 0);
NEW_TASK(blinky2, task2, 0, 100, 0);

sem_t lock = {1, 0};

void setup()
{
    GPIO.output_en = RED_LED_PIN | GREEN_LED_PIN | BLUE_LED_PIN;
    GPIO.output_xor = RED_LED_PIN | GREEN_LED_PIN | BLUE_LED_PIN;
    GPIO.output_val = 0;

    RTC.countlo = 0;
    RTC.counthi = 0;
    RTC.cfg = RTC_ENALWAYS;
}

void task1()
{
    while (1) {
        sem_wait(&lock);
        GPIO.output_val |= RED_LED_PIN;
        for (int i = 0; i < 4000000; i++) asm("");
        GPIO.output_val &= ~RED_LED_PIN;
        for (int i = 0; i < 4000000; i++) asm("");
        sem_signal(&lock);
    }
}

void task2()
{
    while (1)
    {
        sem_wait(&lock);
        GPIO.output_val |= BLUE_LED_PIN;
        for (int i = 0; i < 4000000; i++) asm("");
        GPIO.output_val &= ~BLUE_LED_PIN;
        for (int i = 0; i < 4000000; i++) asm("");
        sem_signal(&lock);
    }
}
