/*
gcc -fno-builtin -nostdlib -std=c17 -c exit.c
ld -nostdlib -nolibc --entry=entry_point -o exit exit.o
*/

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int int16_t;
typedef unsigned short int uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long int int64_t;
typedef unsigned long int uint64_t;
typedef uint64_t size_t;

/*
 * registers used to pass the system call arguments:
 *
 * Arch/ABI  instruction  number  arg1  arg2  arg3  arg4  arg5  arg6  arg7  Ret  Ret
 * x86-64    syscall      rax     rdi   rsi   rdx   r10   r8    r9    -     rax  rdx
 *
 * [ man 2 syscall ]
 *
 * syscall numbers:
 * [ /usr/src/linux-headers-???/arch/x86/include/generated/asm/syscalls_64.h ]
 * [ https://github.com/torvalds/linux/blob/master/arch/x86/entry/syscalls/syscall_64.tbl ]
 *
 */

#define SYSCALL_WRITE 1
#define SYSCALL_EXIT 60

__attribute__((noreturn)) void syscall_exit(uint64_t exit_code) {
  __asm__ volatile (
    "syscall \n"
    : /* no output */
    : /* rax */ "a" (SYSCALL_EXIT),
      /* rdi */ "D" (exit_code)
  );
  __builtin_unreachable();
}

/* syscall entry:
 * 64-bit SYSCALL saves rip to rcx, clears rflags.RF, then saves rflags to r11
 *
 * [ https://github.com/torvalds/linux/blob/master/arch/x86/entry/entry_64.S ]
 *
 */

uint64_t syscall_arg3(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
  uint64_t ret;

  // volatile - has side effects, don't optimize out even if output is unused
  __asm__ volatile (
    "movq %[syscall_number], %%rax \n"
    "movq %[arg1], %%rdi \n"
    "movq %[arg2], %%rsi \n"
    "movq %[arg3], %%rdx \n"
    "syscall \n"
    "movq %%rax, %[ret]"

    : [ret] "=g" (ret)
    : [syscall_number]"g" (syscall_number),
      [arg1] "g" (arg1),
      [arg2] "g" (arg2),
      [arg3] "g" (arg3)
    : /* clobber */ "rax", "rcx", "r11", "rdi", "rsi", "rdx"
  );

  return ret;
}

#define SYS_STDIN  0
#define SYS_STDOUT 1
#define SYS_STDERR 2

size_t strlen(const char* str) {
  const char* p = str;
  while( *p++ );
  return p - str;
}

void entry_point() {
  const char* txt = "Hello, World!\n";
  syscall_arg3(SYSCALL_WRITE, SYS_STDOUT, (uint64_t)txt, strlen(txt));
  syscall_exit(21);
}
