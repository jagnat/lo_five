
    .section .entry
    .global __reset
    .type __reset,@function
__reset:
    lui sp, 0x80004
    call __init
