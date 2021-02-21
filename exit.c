/*
gcc -fno-builtin -nostdlib -c exit.c
ld -nostdlib -nolibc --entry=entry_point -o exit exit.o
*/

typedef long long int64;
typedef unsigned long long uint64;

/*
 * registers used to pass the system call arguments:
 *
 * Arch/ABI arg1  arg2  arg3  arg4  arg5  arg6  arg7
 * x86-64   rdi   rsi   rdx   r10   r8    r9    -
 *
 * Arch/ABI  Instruction  System  Ret  Ret  Error
 * x86-64    syscall      rax     rax  rdx  -
 * 
 * [ man 2 syscall ]
 *
 * syscall numbers:
 * [ /usr/src/linux-headers-???/arch/x86/include/generated/asm/syscalls_64.h ]
 * [ https://github.com/torvalds/linux/blob/master/arch/x86/entry/syscalls/syscall_64.tbl ]
 *
 * * syscall entry:
 * 64-bit SYSCALL saves rip to rcx, clears rflags.RF, then saves rflags to r11
 *
 * [ https://github.com/torvalds/linux/blob/master/arch/x86/entry/entry_64.S ]
 *
 */

#define SYSCALL_EXIT 60

void std_exit(int64 exit_code) {
  // volatile - has side effects, don't optimize out even if output is unused
  asm volatile (
    "movq %0, %%rax \n"
    "movq %1, %%rdi \n"
    "syscall \n"

      : /* no output */
      : /* input %0 */ "i" (SYSCALL_EXIT),
        /* input %1 */ "g" (exit_code)
      : /* clobber */ "rax", "rdi", "rcx", "r11"
  );
}

void entry_point() {
  std_exit(21);
}
