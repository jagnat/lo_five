#include "task.h"

#include "hifive1b.h"
#include "interrupt.h"

void __idle_task();

NEW_TASK(idle_task, __idle_task, NUM_TASK_PRIORITIES - 1, 4, 0);
NEW_TASK(main_task, 0, NUM_TASK_PRIORITIES - 1, 128, 0);
void* const __INITIAL_SP = (void*)(&main_task + 1);

task_t *current_task;

static task_t *ready_tasks[NUM_TASK_PRIORITIES];

// The timer frequency for preemptive task scheduling
const int HZ = 1000;

void __systick();
void __schedule();

void enqueue_task(task_t *task, task_t **list);
task_t* dequeue_task(task_t **list);
void add_task_to_ready_queue(task_t* task);
task_t* get_ready_task();

void sem_wait(sem_t *s)
{
    int prev = disable_interrupts();
    if (s->count > 0)
        s->count--;
    else
    {
        enqueue_task(current_task, &s->waiting);
        current_task->state = SUSPENDED;
        yield();
    }
    restore_interrupts(prev);
}

void sem_signal(sem_t *s)
{
    int prev = disable_interrupts();
    if (s->waiting == 0)
        s->count++;
    else
    {
        task_t *released = dequeue_task(&s->waiting);
        released->state = READY;
        add_task_to_ready_queue(released);
        yield();
    }
    restore_interrupts(prev);
}

void sem_signal_from_isr(sem_t *s)
{
    int prev = disable_interrupts();
    if (s->waiting == 0)
        s->count++;
    else
    {
        task_t *released = dequeue_task(&s->waiting);
        released->state = READY;
        add_task_to_ready_queue(released);
        __schedule();
    }
    restore_interrupts(prev);

}

void yield()
{
    int prev = enable_interrupts();
    asm("ecall");
    restore_interrupts(prev);
}

void __init_tasks()
{
    extern task_t* __TASKS_ADDR;
    extern task_t* __TASKS_END;

    task_t **task_walk = &__TASKS_ADDR;

    while (task_walk != &__TASKS_END)
    {
        if (*task_walk != __main_task_ptr && *task_walk != __idle_task_ptr)
        {
            add_task_to_ready_queue(*task_walk);
        }
        task_walk++;
    }

    current_task = __main_task_ptr;

    CLINT.mtimecmp = 32768 / HZ;

    timer_set_handler(__systick);
    ecall_set_handler(__schedule);
}

void __idle_task()
{
    while (1)
    {
        asm("wfi");
    }
}

void __systick()
{    
    __schedule();
    CLINT.mtime = 0;
}

void __schedule()
{
    task_t *prev = current_task;

    if (current_task->state == READY && current_task != &idle_task.task)
    {
        add_task_to_ready_queue(current_task);
    }

    current_task = get_ready_task();
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

void add_task_to_ready_queue(task_t *task)
{
    enqueue_task(task, ready_tasks + task->priority);
}

task_t* get_ready_task()
{
    for (int i = 0; i < NUM_TASK_PRIORITIES; i++)
    {
        if (ready_tasks[i])
            return dequeue_task(ready_tasks + i);
    }
    return &idle_task.task;
}

