#ifndef __TASK_H__
#define __TASK_H__

typedef struct _task_t task_t;
struct _task_t
{
    task_t *next;
    unsigned *sp;
    unsigned short priority;
    unsigned short effective_priority;
};

#define TASK(name, func, priority, stacksize, arg) \
struct { \
    task_t task; \
    unsigned stack[stacksize]; \
    unsigned x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15; \
    unsigned x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31; \
} static name = { \
    {0, &(name.x2), priority, priority}, \
    {0}, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, arg, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, func \
};

#endif // __TASK_H__
