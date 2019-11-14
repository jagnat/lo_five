
#include "hifive1b.h"

__attribute__((section(".entry")))
void __entry ()
{
    GPIO.output_en = BIT(21);
    GPIO.output_val = BIT(21);
    while (1)
    {
        asm("");
        GPIO.output_val = BIT(21);
    }
}
