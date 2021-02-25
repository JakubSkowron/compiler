    .global entry_point

    .text

entry_point:
    endbr64             # mark as a valid target for indirect branch (Intel CET)

    movq %rdx, finalize_ptr # save %rdx, function pointer to be registered with atexit
                            # (finalizer for shared libraries)

    mov %rsp, %rbp      # save $rsp for later
    andq $~0x0F, %rsp   # allign stack to 16 bytes
    mov %rsp, %rdi      # 1st parameter: stack pointer at entry
                        #                (points to argc, *argv, *envp)
    call c_entry_point
    mov %rbp, %rsp      # restore stack
    mov $exit_point, %rdx  # setup my own finalizer
    xor %rbp, %rbp      # stack frame linked list terminator
    jmp _start          # jump to standard libc entry point

    # could be written in C, I think
exit_point:
    push %rbp           # align stack to 16 bytes
    movq $finalize_ptr, %rax
    cmpq $0, (%rax)
    je .skip_finalize
    call *finalize_ptr
.skip_finalize:
    call c_exit_point
    pop %rbp
    ret

    .data

finalize_ptr:
    .quad 0

