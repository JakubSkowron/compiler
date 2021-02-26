# X86-64 Calling Convention (x86-64 System V ABI)

Based on [System V Application Binary Interface draft](https://gitlab.com/x86-psABIs/x86-64-ABI).

Run `make pdf`. You need `texlive-latex-extra` package.

Function parameters:
|      |      |      |      |      |      |
|------|------|------|------|------|------|
| arg1 | arg2 | arg3 | arg4 | arg5 | arg6 |
| rdi  | rsi  | rdx  | rcx  | r8   | r9   |

Return value in `rax` or `rax,rdx`

More parameters are pushed to the stack in reverse order.

Registers not clobbered by called function (callee-saved): `rbx`, `rsp`, `rbp`, `r12` to `r15`.
These registers "belong" to the calling function. Called function needs to restore their
values before returning, and can change all the others.

Note that in contrast to the Intel386 ABI, `%rdi` and `%rsi` belong to the called function, not the caller.

## `call` instruction

`call` instruction pushes address of the next instruction to the stack, and does jump.
Just before `call` stack has to be 16-byte aligned.

`ret` instruction pops a value from the stack and jumps to it.

Note: `push` first decrements `%rsp` and then places value at `(%rsp)`, so at entry to function stack is no longer 16-bytes aligned.

At `(%rsp)` is return addres, so if (optionally) function preambule pushes `%rbp`, then `%rsp` is
aligned to 16 byte boundary.

## Register Usage
(Figure 3.4)

|    |    |
|----|----|
| rax    | temporary register <br/>1 st return register <br/> with variable arguments passes information about the number of vector registers used |
| rbx    | callee-saved register |
| rcx    | used to pass 4 th integer argument to functions |
| rdx    | used to pass 3 rd argument to functions <br/> 2 nd return register |
| rsp    | stack pointer (callee-saved) |
| rbp    | optionally used as frame pointer (callee-saved) |
| rsi    | used to pass 2 nd argument to functions |
| rdi    | used to pass 1 st argument to functions |
| r8     | used to pass 5 th argument to functions |
| r9     | used to pass 6 th argument to functions |
| r10    | temporary register <br/> used for passing a function’s static chain pointer |
| r11    | temporary register |
| r12 - r14 | callee-saved registers |
| r15    | optionally used as GOT base pointer (callee-saved) |

## Stack frame with Base Pointer

This is optional &mdash; but usefull for debuggers. `%rbp` creates a linked list of stack frames.

| address      | value             |
|-------------:|-------------------|
|              | ...               |
| `24(%rbp)`   | memory argument 2 |
| `16(%rbp)`   | memory argument 1 |
|         -    | previous frame ↑, current frame ↓ |
| `8(%rbp)`    | return address    |
| `(%rbp)`     | previous `%rbp` value |
| `-8(%rbp)`   | local variable 1  |
| `-16(%rbp)`  | local variable 2  |
|              | ...               |
| `(%rsp)`     | top of the stack  |
|              | ...               |
| `-128(%rsp)` | red zone          |


The _red zone_, 128-byte area beyond the location pointed to by %rsp, is reserved
and will not be modified by signal or interrupt handlers. Functions may use
this area for temporary data that is not needed across function calls (leaf
functions may use this area for their entire stack frame).

Note: sizes of local variables can be different, not necessarily 8 bytes, but
size of each memory argument is rounded up to 8 bytes.

`leave` moves %rbp to %rsp, and pops top of stack into %rbp, so restores previous stack frame. Equivalent to:
```asm
movq %rbp, %rsp
popq %rbp
```

# Linux syscall convention

Registers used to pass the system call arguments:

|             |      |      |      |      |      |      |
|-------------|------|------|------|------|------|------|
| call number | arg1 | arg2 | arg3 | arg4 | arg5 | arg6 |
| rax         | rdi  | rsi  | rdx  | r10  | r8   | r9   |

Return value in rax.

Syscall clobbers rcx and r11 (<https://github.com/torvalds/linux/blob/master/arch/x86/entry/entry_64.S>):
> 64-bit SYSCALL saves rip to rcx, clears rflags.RF, then saves rflags to r11

* syscall numbers: <https://github.com/torvalds/linux/blob/master/arch/x86/entry/syscalls/syscall_64.tbl>
* syscall headers: <https://github.com/torvalds/linux/blob/master/include/linux/syscalls.h>

# Linux program entry

Source glibc: [sysdeps/x86_64/start.S](https://sourceware.org/git/?p=glibc.git;a=blob;f=sysdeps/x86_64/start.S)

* `%rdx` Contains a function pointer to be registered with `atexit`. This is how the dynamic linker arranges to have DT_FINI functions called for shared libraries that have been loaded before this code runs.
* `%rsp` The stack contains the arguments (`argc`, `argv`) and environment (`envp`):

| address | value   |
|--------:|---------|
| `(%rsp)` | argc    |
| `8(%rsp)` | argv[0] |
|         | ...     |
| `(%rsp, argc, 8)` | NULL    |
| `8(%rsp, argc, 8)` | envp[0] |
|         | ...     |
|         | NULL    |