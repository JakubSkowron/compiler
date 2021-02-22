Exploring compiling code with clang/gcc, but without standard libraries:
```
cc -fno-builtin -nostdlib -std=c17
ld -nostdlib -nolibc --entry=xxxx
```
System resources need to be accessed by syscall.
