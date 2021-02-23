    .global entry_point

    .text
entry_point:
    # above stack pointer is argc, argv
    leaq 8(%rsp), %rdi  # argv
    jmp c_entry_point
