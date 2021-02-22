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

_Static_assert(sizeof(int8_t) == 1, "");
_Static_assert(sizeof(uint8_t) == 1, "");
_Static_assert(sizeof(int16_t) == 2, "");
_Static_assert(sizeof(uint16_t) == 2, "");
_Static_assert(sizeof(int32_t) == 4, "");
_Static_assert(sizeof(uint32_t) == 4, "");
_Static_assert(sizeof(int64_t) == 8, "");
_Static_assert(sizeof(uint64_t) == 8, "");

typedef _Bool bool;

/*
 * registers used to pass the system call arguments:
 *
 * Arch/ABI  instruction  number  arg1  arg2  arg3  arg4  arg5  arg6  arg7  Ret
 * Ret x86-64    syscall      rax     rdi   rsi   rdx   r10   r8    r9    - rax
 * rdx
 *
 * [ man 2 syscall ]
 *
 * syscall numbers:
 * [ /usr/src/linux-headers/arch/x86/include/generated/asm/syscalls_64.h ]
 * [ https://github.com/torvalds/linux/blob/master/arch/x86/entry/syscalls/syscall_64.tbl ]
 *
 */

#define SYSCALL_WRITE 1
#define SYSCALL_BRK 12
#define SYSCALL_EXIT 60

__attribute__((noreturn)) void syscall_exit(uint64_t exit_code) {
  // GCC Extended Assembler Template
  // [ https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html ]
  // volatile - has side effects, don't optimize out even if output is unused
  __asm__ volatile (
    "syscall \n"
    : /* no output */
    : /* rax */ "a" (SYSCALL_EXIT),
      /* rdi */ "D" (exit_code)
  );
  __builtin_unreachable();
}

uint64_t syscall_arg1(uint64_t syscall_number, uint64_t arg1) {
  uint64_t ret;

  __asm__ volatile("syscall \n"
                   "movq %%rax, %[ret]"

                   : [ret] "=g"(ret)
                   : /* rax */ "a"(syscall_number),
                     /* rdi */ "D"(arg1)
                   : /* clobber */ "rcx", "r11");

  // syscall clobbers rcx and r11:
  // "64-bit SYSCALL saves rip to rcx, clears rflags.RF, then saves rflags to r11"
  // [ https://github.com/torvalds/linux/blob/master/arch/x86/entry/entry_64.S ]

  return ret;
}

uint64_t syscall_arg2(uint64_t syscall_number, uint64_t arg1, uint64_t arg2) {
  uint64_t ret;

  __asm__ volatile("syscall \n"
                   "movq %%rax, %[ret]"

                   : [ret] "=g"(ret)
                   : /* rax */ "a"(syscall_number),
                     /* rdi */ "D"(arg1),
                     /* rsi */ "S"(arg2)
                   : /* clobber */ "rcx", "r11");

  return ret;
}

uint64_t syscall_arg3(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
  uint64_t ret;

  __asm__ volatile("syscall \n"
                   "movq %%rax, %[ret]"

                   : [ret] "=g"(ret)
                   : /* rax */ "a"(syscall_number),
                     /* rdi */ "D"(arg1),
                     /* rsi */ "S"(arg2),
                     /* rdx */ "d"(arg3)
                   : /* clobber */ "rcx", "r11");

  return ret;
}

uint64_t syscall_arg4(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
  uint64_t ret;

  // There is no constraint on this register in GCC assembler template
  // [ https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html#Machine-Constraints ]
  register uint64_t r10 __asm__("r10") = arg4;

  __asm__ volatile("syscall \n"
                   "movq %%rax, %[ret]"

                   : [ret] "=g"(ret)
                   : /* rax */ "a"(syscall_number),
                     /* rdi */ "D"(arg1),
                     /* rsi */ "S"(arg2),
                     /* rdx */ "d"(arg3), "r"(r10)
                   : /* clobber */ "rcx", "r11");

  return ret;
}

uint64_t syscall_arg5(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
                      uint64_t arg5) {
  uint64_t ret;

  register uint64_t r10 __asm__("r10") = arg4;
  register uint64_t r8 __asm__("r8") = arg5;

  __asm__ volatile("syscall \n"
                   "movq %%rax, %[ret]"

                   : [ret] "=g"(ret)
                   : /* rax */ "a"(syscall_number),
                     /* rdi */ "D"(arg1),
                     /* rsi */ "S"(arg2),
                     /* rdx */ "d"(arg3), "r"(r10), "r"(r8)
                   : /* clobber */ "rcx", "r11");

  return ret;
}

uint64_t syscall_arg6(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
                      uint64_t arg5, uint64_t arg6) {
  uint64_t ret;

  register uint64_t r10 __asm__("r10") = arg4;
  register uint64_t r8 __asm__("r8") = arg5;
  register uint64_t r9 __asm__("r9") = arg6;

  __asm__ volatile("syscall \n"
                   "movq %%rax, %[ret]"

                   : [ret] "=g"(ret)
                   : /* rax */ "a"(syscall_number),
                     /* rdi */ "D"(arg1),
                     /* rsi */ "S"(arg2),
                     /* rdx */ "d"(arg3), "r"(r10), "r"(r8), "r"(r9)
                   : /* clobber */ "rcx", "r11");

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

void assert(bool condition, const char *msg) {
  if (condition)
    return;
  syscall_arg3(SYSCALL_WRITE, SYS_STDERR, (uint64_t)msg, strlen(msg));
  syscall_exit(1);
}

void print_hex(uint64_t x) {
  const char *hex = "0123456789ABCDEF";
  char buf[17];
  char *p = buf + 16;
  *p-- = '\n';
  while (1) {
    *p = hex[x & 0xF];
    x >>= 4;
    if (p == buf)
      break;
    --p;
  }
  uint64_t ret = syscall_arg3(SYSCALL_WRITE, SYS_STDOUT, (uint64_t)buf, sizeof(buf));
  assert(ret == sizeof(buf), "write failed\n");
}

void entry_point() {
  const char* txt = "Hello, World!\n";
  syscall_arg3(SYSCALL_WRITE, SYS_STDOUT, (uint64_t)txt, strlen(txt));
  void *mem = (void *)syscall_arg1(SYSCALL_BRK, 0);
  print_hex((uint64_t)mem);
  void *mem_end = (void *)syscall_arg1(SYSCALL_BRK, (uint64_t)((char *)mem + 100));
  print_hex((uint64_t)mem_end);
  int *i = ((int *)mem) + 15;
  *i = 0xABCD1234;
  print_hex((uint64_t)i);
  print_hex((uint64_t)*i);
  syscall_exit(0);
}
