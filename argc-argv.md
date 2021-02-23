# Program parameters

At program entry point `argc` and `argv` are just above the stack. To be , i.e. in `(%rsp)` and `8(%rsp)`.

So this below loads value of `argv` to `%rax`
```as
    leaq 8(%rsp), %rax  # argv
```

# In kernel

When new process is started `argc`, `argv` are copied to the top of new process's memory.

Code from kernel here:
[fs/exec.c](https://github.com/torvalds/linux/blob/master/fs/exec.c#L513)
```c
/*
 * 'copy_strings()' copies argument/environment strings from the old
 * processes's memory to the new process's stack.  The call to get_user_pages()
 * ensures the destination page is created and not swapped out.
 */
static int copy_strings(int argc, struct user_arg_ptr argv,
      struct linux_binprm *bprm)
    ...
    pos = bprm->p;
    ...
}
```

[include/linux/binfmts.h](https://github.com/torvalds/linux/blob/master/include/linux/binfmts.h#L17)
```c
struct linux_binprm {
  ...
  unsigned long p; /* current top of mem */
  ...
};
```

You can see your memory map using:
```
$ cat /proc/self/maps
...
56275a9d7000-56275a9f8000 rw-p 00000000 00:00 0                          [heap]
...
7ffdb117b000-7ffdb119c000 rw-p 00000000 00:00 0                          [stack]
```
Stack grows down: next element has lower address.