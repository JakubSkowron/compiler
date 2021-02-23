    .global entry_point

    .text
entry_point:
    movq %rsp, %rdi     # pass stack pointer at entry as argument for C function
    jmp c_entry_point
