#include "hifive1b.h"
#include "task.h"

void task2();

NEW_TASK(blinky, task2, 0, 100, 0);

void setup()
{
    GPIO.output_en = BIT(21) | BIT(19);
    GPIO.output_val = BIT(21) | BIT(19);

    RTC.countlo = 0;
    RTC.counthi = 0;
    RTC.cfg = RTC_ENALWAYS;
}

void loop()
{
    while (RTC.countlo % 65536 < 32768) asm("");
    GPIO.output_val &= ~BIT(21);
    while (RTC.countlo % 65536 >= 32768) asm("");
    GPIO.output_val |= BIT(21);
}

void task2()
{
    while (1)
    {
        while (RTC.countlo % 32768 < 16384) asm("");
        GPIO.output_val &= ~BIT(19);
        while (RTC.countlo % 32768 >= 16384) asm("");
        GPIO.output_val |= BIT(19);
    }
}
