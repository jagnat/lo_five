#include "task.h"

#include "hifive1b.h"
#include "interrupts.h"

NEW_TASK(main_task, 0, 0, 221, 0);
void* const __INITIAL_SP = (void*)(&main_task + 1);

task_t *current_task;

static task_t *ready_tasks;

// The timer frequency for preemptive task scheduling
const int HZ = 100;

void __systick();

void __init_tasks()
{
    extern task_t* __TASKS_ADDR;
    extern task_t* __TASKS_END;

    task_t **task_walk = &__TASKS_ADDR;

    while (task_walk != &__TASKS_END)
    {
        if (*task_walk != __main_task_ptr)
        {
            enqueue_task(*task_walk);
        }
        task_walk++;
    }

    current_task = __main_task_ptr;

    CLINT.mtimecmp = 32768 / HZ;

    timer_set_handler(__systick);
}

void __systick()
{    
    schedule();
    CLINT.mtime = 0;
}

void schedule()
{
    // TODO: Should we disable interrupts?

    task_t *prev = current_task;

    if (current_task)
    {
        enqueue_task(current_task);
    }

    current_task = dequeue_task();
}

// TODO: Make this no be an O(n) operation
void enqueue_task(task_t *task)
{
    task->next = 0;
    if (!ready_tasks)
        ready_tasks = task;
    else
    {
        task_t *iter = ready_tasks;
        while (iter->next) { iter = iter->next; }
        iter->next = task;
    }
}

task_t* dequeue_task()
{
    task_t *res = 0;
    if (ready_tasks != 0)
    {
        res = ready_tasks;
        ready_tasks = ready_tasks->next;
        res->next = 0;
    }

    return res;
}


