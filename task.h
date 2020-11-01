#pragma once

typedef enum _task_state_e task_state_e;
enum _task_state_e
{
    READY = 0,
    SUSPENDED = 1,
    COMPLETE = 2,
};

#define NUM_TASK_PRIORITIES 4

typedef struct _task_t task_t;
struct _task_t
{
    task_t *next;
    unsigned *sp;
    task_state_e state; 
    unsigned short priority;
    unsigned short effective_priority;
};

typedef struct
{
    unsigned count;
    task_t *waiting;
} sem_t;

void sem_wait(sem_t *s);
void sem_signal(sem_t *s);
void sem_signal_from_isr(sem_t *s);

void yield();

/* This macro defines a new task with the specified parameters,
 * which will then be scheduled to run alongside the main task.
 * The task will run the void procedure(void*) function, which
 * should never return.
 */
#define NEW_TASK(name, procedure, priority, stacksize, arg) \
struct { \
    task_t task; \
    unsigned stack[stacksize]; \
    unsigned pc,  x1,  x2,  x3; \
    unsigned x4,  x5,  x6,  x7; \
    unsigned x8,  x9,  x10, x11; \
    unsigned x12, x13, x14, x15; \
    unsigned x16, x17, x18, x19; \
    unsigned x20, x21, x22, x23; \
    unsigned x24, x25, x26, x27; \
    unsigned x28, x29, x30, x31; \
    unsigned mcause, mstatus, mscratch; \
} static name = { \
    {0, &(name.pc), READY, priority, priority}, \
    {0}, \
    (unsigned)procedure, 0, (unsigned)(&(name.pc)), 0, \
    0, 0, 0, 0, \
    0, 0, arg, 0, \
    0, 0, 0, 0, \
    0, 0, 0, 0, \
    0, 0, 0, 0, \
    0, 0, 0, 0, \
    0, 0, 0, 0, \
    0, 0, 0, \
}; \
void* const __##name##_ptr __attribute((section(".tasks"))) = &name;

