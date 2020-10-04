.extern current_task
.extern __INITIAL_SP
    
.global __reset
.global __irq_proc

.macro SAVE_CTX
    addi sp, sp, -35*4
    sw x1,  1 *4(sp)
    sw x2,  2 *4(sp)
    sw x3,  3 *4(sp)
    sw x4,  4 *4(sp)
    sw x5,  5 *4(sp)
    sw x6,  6 *4(sp)
    sw x7,  7 *4(sp)
    sw x8,  8 *4(sp)
    sw x9,  9 *4(sp)
    sw x10, 10*4(sp)
    sw x11, 11*4(sp)
    sw x12, 12*4(sp)
    sw x13, 13*4(sp)
    sw x14, 14*4(sp)
    sw x15, 15*4(sp)
    sw x16, 16*4(sp)
    sw x17, 17*4(sp)
    sw x18, 18*4(sp)
    sw x19, 19*4(sp)
    sw x20, 20*4(sp)
    sw x21, 21*4(sp)
    sw x22, 22*4(sp)
    sw x23, 23*4(sp)
    sw x24, 24*4(sp)
    sw x25, 25*4(sp)
    sw x26, 26*4(sp)
    sw x27, 27*4(sp)
    sw x28, 28*4(sp)
    sw x29, 29*4(sp)
    sw x30, 30*4(sp)
    sw x31, 31*4(sp)
    csrr t0, mcause
    sw t0, 32*4(sp)
    csrr t0, mstatus
    sw t0, 33*4(sp)
    csrr t0, mscratch
    sw t0, 34*4(sp)

    # Store sp in current task block
    lw t0, current_task
    sw sp, 4(t0)
.endm

# Check MSB of mcause, if it is clear, we are in
# a HW exception, so MEPC points to the last instruction
# that occurred. So we need to increment MEPC by 4
# to point to the next instruction that should run.
.macro SAVE_EPC
    csrr t0, mepc
    csrr t1, mcause
    srli t1, t1, 31
    bne  t1, x0, __hw_int
    addi t0, t0, 4
__hw_int:
    sw t0, 0*4(sp)
.endm

.macro LOAD_CTX
    # Get sp from current_task.sp
    lw t0, current_task
    lw sp, 4(t0)

    # Load pc from top of current task stack
    lw t0,  0 *4(sp)
    csrw mepc, t0

    # Load mstatus
    #lw t0, 33*4(sp)
    #csrw mstatus, t0

    # Load the rest of the regs
    lw x1,  1 *4(sp)
    lw x2,  2 *4(sp)
    lw x3,  3 *4(sp)
    lw x4,  4 *4(sp)
    lw x5,  5 *4(sp)
    lw x6,  6 *4(sp)
    lw x7,  7 *4(sp)
    lw x8,  8 *4(sp)
    lw x9,  9 *4(sp)
    lw x10, 10*4(sp)
    lw x11, 11*4(sp)
    lw x12, 12*4(sp)
    lw x13, 13*4(sp)
    lw x14, 14*4(sp)
    lw x15, 15*4(sp)
    lw x16, 16*4(sp)
    lw x17, 17*4(sp)
    lw x18, 18*4(sp)
    lw x19, 19*4(sp)
    lw x20, 20*4(sp)
    lw x21, 21*4(sp)
    lw x22, 22*4(sp)
    lw x23, 23*4(sp)
    lw x24, 24*4(sp)
    lw x25, 25*4(sp)
    lw x26, 26*4(sp)
    lw x27, 27*4(sp)
    lw x28, 28*4(sp)
    lw x29, 29*4(sp)
    lw x30, 30*4(sp)
    lw x31, 31*4(sp)

    addi sp, sp, 35*4
.endm

.section .entry
.align 2
__reset:
    lw sp, __INITIAL_SP
    call __init

.section .text
.align 2
__irq_proc:
    SAVE_CTX
    SAVE_EPC
    # Pass cause as first arg to handler
    mv a0,sp 
    call __irq_handler
    LOAD_CTX
    mret

