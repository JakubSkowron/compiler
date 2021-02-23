# Standalone program
Exploring compiling code with clang/gcc, but without standard libraries. System resources need to be accessed by syscalls.
```
cc -ffreestanding
ld -nostdlib -nolibc --entry=...
```
## Compiler options

* -ffreestanding
      Assert that compilation targets a freestanding environment.  This implies
      -fno-builtin.  A freestanding environment is one in which the standard
      library may not exist, and program startup may not necessarily be at
      "main".
* -fno-builtin
      Don't recognize built-in functions that do not begin with `__builtin_`

With `-ffreestanding` compiler only provide certain library facilities:
those in `float.h`, `limits.h`, `stdarg.h`, `stddef.h`, `iso646.h`, `stdbool.h`, `stdint.h`,
`stdalign.h`, `stdnoreturn.h`. So you can use only these header files.

GCC requires the freestanding environment provide `memcpy`, `memmove`, `memset` and `memcmp`.
You need to provide them, because compiler may generate calls to them.

## Linker options
* -nostdlib
     Do not use the standard system startup files or libraries when linking.
So it implies:
* -nodefaultlibs
    Do not use the standard system libraries when linking.
    The compiler may generate calls to "memcmp", "memset", "memcpy" and
    "memmove".  These entries are usually resolved by entries in libc.  These
    entry points should be supplied through some other mechanism when this
    option is specified.
* -nostartfiles
    Do not use the standard system startup files when linking.
* -nolibc
    Do not use the C library or system libraries tightly coupled with it when
    linking. 

`libgcc.a` is a library of internal subroutines which GCC uses to overcome
shortcomings of particular machines, or special needs for some languages.
For example: no division instruction on ARM, so GCC will emit call to `__aeabi_idiv`.
Add `-lgcc` in such case.

## Program entry

Note, that C function preambule can modify stack pointer. We loose original `%rsp` if
compiler chooses to change it.
If we want to have access to `argc`, `argv`, `envp` in C function we need to save `$rsp`
and pass it as argument (in register `%rdi`):
```asm
movq %rsp, %rdi
jmp c_entry_point  # jump, no return
```
C function header:
```c
__attribute__((noreturn)) void c_entry_point(void* entry_stack);
```