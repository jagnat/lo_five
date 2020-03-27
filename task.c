#include "task.h"

#include "hifive1b.h"
#include "interrupts.h"

NEW_TASK(main_task, 0, 0, 100, 0);
void* const __INITIAL_SP = (void*)(&main_task + 1);

task_t *current_task;

// The timer frequency for preemptive task scheduling
const int HZ = 100;

void __systick();

void __init_tasks()
{
    current_task = __main_task_ptr;
    GPIO.output_en = BIT(22);
    GPIO.output_val = BIT(22);

    CLINT.mtimecmp = 32768 / HZ;

    timer_set_handler(__systick);
}

void __systick()
{    
    static int i = 0;
    if (i == 0)
        GPIO.output_val = 0;
    else if (i == 1)
        GPIO.output_val = BIT(22);
    else if (i == HZ)
        i = -1;
    i++;
    CLINT.mtime = 0;
}

