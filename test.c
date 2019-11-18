
#include "hifive1b.h"

__attribute__((section(".entry")))
void __entry ()
{
    GPIO.input_en = GPIO.input_en & ~(BIT(22) | BIT(19) | BIT(21));
    GPIO.output_en = BIT(22) | BIT(19) | BIT(21);
    GPIO.output_val |= BIT(22) | BIT(19) | BIT(21);
    GPIO.out_xor &= ~(BIT(22) | BIT(19) | BIT(21));
    while (1)
    {
        GPIO.output_val |= BIT(22) | BIT(19) | BIT(21);
        for (int i = 0; i < 3000; i++)
          asm("");
        GPIO.output_val &= ~(BIT(22) | BIT(19) | BIT(21));
        for (int i = 0; i < 30; i++)
          asm("");

    }
}
