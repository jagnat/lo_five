#include "task.h"

#include "hifive1b.h"
#include "interrupts.h"

NEW_TASK(main_task, 0, 0, 221, 0);
void* const __INITIAL_SP = (void*)(&main_task + 1);

task_t *current_task;

static task_t *ready_tasks;

// The timer frequency for preemptive task scheduling
const int HZ = 1000;

void __systick();
void enqueue_task(task_t *task, task_t **list);
task_t *dequeue_task(task_t **list);

void __init_tasks()
{
    extern task_t* __TASKS_ADDR;
    extern task_t* __TASKS_END;

    task_t **task_walk = &__TASKS_ADDR;

    while (task_walk != &__TASKS_END)
    {
        if (*task_walk != __main_task_ptr)
        {
            enqueue_task(*task_walk, &ready_tasks);
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

    if (current_task->state == READY)
    {
        enqueue_task(current_task, &ready_tasks);
    }

    current_task = dequeue_task(&ready_tasks);
}

// TODO: Make this no be an O(n) operation
void enqueue_task(task_t *task, task_t **list)
{
    task->next = 0;
    if (!(*list))
        *list = task;
    else
    {
        task_t *iter = *list;
        while (iter->next) { iter = iter->next; }
        iter->next = task;
    }
}

task_t* dequeue_task(task_t **list)
{
    task_t *res = 0;
    if (*list != 0)
    {
        res = *list;
        *list = res->next;
        res->next = 0;
    }

    return res;
}

void sem_wait(sem_t *s)
{
    disable_interrupts();
    if (s->count > 0)
        s->count--;
    else
    {
        enqueue_task(current_task, &s->waiting);
        current_task->state = SUSPENDED;
        asm("ecall");
    }
    enable_interrupts();
}

void sem_signal(sem_t *s)
{
    disable_interrupts();
    if (s->waiting == 0)
        s->count++;
    else
    {
        task_t *released = dequeue_task(&s->waiting);
        released->state = READY;
        enqueue_task(released, &ready_tasks);
        asm("ecall");
    }
    enable_interrupts();
}

void mut_lock(mut_t *m)
{
}

void mut_unlock(mut_t *m)
{
}

