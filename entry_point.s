    .global entry_point

    .text

entry_point:
    endbr64             # mark as a valid target for indirect branch (Intel CET)

    xor %rbp, %rbp      # %rbp = 0, stack frame linked list terminator. This is so debugger
                        # knows when to stop looking frame up, if c_entry_point chooses
                        # to push %rbp in preambule

    mov %rsp, %rdi      # 1st parameter: stack pointer at entry
                        #                (points to argc, *argv, *envp)
    mov %rdx, %rsi      # 2nd parameter: function pointer to be registered with atexit
                        #                (finalizer for shared libraries)

    andq $~0x0F, %rsp   # align stack to 16 bytes
    call c_entry_point
    hlt                 # guard in case the function returns, only kernel can hlt, segfault
