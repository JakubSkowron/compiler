/*
cc -ffreestanding
ld -nostdlib -nolibc --entry=...
*/

#include "syscall_64.h"

/*
 * registers used to pass the system call arguments:
 *
 * instruction  call number  arg1  arg2  arg3  arg4  arg5  arg6
 * syscall      rax          rdi   rsi   rdx   r10   r8    r9
 *
 * return value in rax or rax,rdx (actually, I don't think any syscall uses rdx)
 *
 * [ man 2 syscall ]
 */

__attribute__((noreturn)) void syscall_exit(uint32_t exit_code) {
  // GCC Extended Assembler Template
  // [ https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html ]
  __asm__ volatile("syscall \n"
                   : /* no output */
                   : /* rax */ "a"(SYSCALL_EXIT),
                     /* rdi */ "D"(exit_code));
  __builtin_unreachable();
}

uint64_t syscall_arg1(uint64_t syscall_number, uint64_t arg1) {
  // using local register variable to force operand into register
  // [ https://gcc.gnu.org/onlinedocs/gcc-6.2.0/gcc/Local-Register-Variables.html ]
  register uint64_t ret __asm__("rax");

  // volatile - has side effects, don't optimize out even if output is unused
  __asm__ volatile("syscall \n"
                   : /* rax */ "=a"(ret)
                   : /* rax */ "0"(syscall_number),
                     /* rdi */ "D"(arg1)
                   : /* clobber */ "rcx", "r11");

  // syscall clobbers rcx and r11:
  // "64-bit SYSCALL saves rip to rcx, clears rflags.RF, then saves rflags to r11"
  // [ https://github.com/torvalds/linux/blob/master/arch/x86/entry/entry_64.S ]

  return ret;
}

uint64_t syscall_arg2(uint64_t syscall_number, uint64_t arg1, uint64_t arg2) {
  register uint64_t ret __asm__("rax");

  __asm__ volatile("syscall \n"
                   : /* rax */ "=a"(ret)
                   : /* rax */ "0"(syscall_number),
                     /* rdi */ "D"(arg1),
                     /* rsi */ "S"(arg2)
                   : /* clobber */ "rcx", "r11");
  return ret;
}

uint64_t syscall_arg3(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
  register uint64_t ret __asm__("rax");

  __asm__ volatile("syscall \n"
                   : /* rax */ "=a"(ret)
                   : /* rax */ "0"(syscall_number),
                     /* rdi */ "D"(arg1),
                     /* rsi */ "S"(arg2),
                     /* rdx */ "d"(arg3)
                   : /* clobber */ "rcx", "r11");

  return ret;
}

uint64_t syscall_arg4(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
  register uint64_t ret __asm__("rax");

  // Using local register variable, because there is no constraint on this register
  // in GCC assembler template
  // [ https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html#Machine-Constraints ]
  register uint64_t r10 __asm__("r10") = arg4;

  __asm__ volatile("syscall \n"
                   : /* rax */ "=a"(ret)
                   : /* rax */ "0"(syscall_number),
                     /* rdi */ "D"(arg1),
                     /* rsi */ "S"(arg2),
                     /* rdx */ "d"(arg3), "r"(r10)
                   : /* clobber */ "rcx", "r11");

  return ret;
}

uint64_t syscall_arg5(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
                      uint64_t arg5) {
  register uint64_t ret __asm__("rax");

  register uint64_t r10 __asm__("r10") = arg4;
  register uint64_t r8 __asm__("r8") = arg5;

  __asm__ volatile("syscall \n"
                   : /* rax */ "=a"(ret)
                   : /* rax */ "0"(syscall_number),
                     /* rdi */ "D"(arg1),
                     /* rsi */ "S"(arg2),
                     /* rdx */ "d"(arg3), "r"(r10), "r"(r8)
                   : /* clobber */ "rcx", "r11");

  return ret;
}

uint64_t syscall_arg6(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
                      uint64_t arg5, uint64_t arg6) {
  register uint64_t ret __asm__("rax");

  register uint64_t r10 __asm__("r10") = arg4;
  register uint64_t r8 __asm__("r8") = arg5;
  register uint64_t r9 __asm__("r9") = arg6;

  __asm__ volatile("syscall \n"
                   : /* rax */ "=a"(ret)
                   : /* rax */ "0"(syscall_number),
                     /* rdi */ "D"(arg1),
                     /* rsi */ "S"(arg2),
                     /* rdx */ "d"(arg3), "r"(r10), "r"(r8), "r"(r9)
                   : /* clobber */ "rcx", "r11");

  return ret;
}

size_t syscall_read(uint32_t fd, char *buf, size_t count) {
  return syscall_arg3(SYSCALL_READ, fd, (uint64_t)buf, count);
}

size_t syscall_write(uint32_t fd, const char *buf, size_t count) {
  return syscall_arg3(SYSCALL_WRITE, fd, (uint64_t)buf, count);
}

uint64_t syscall_stat(const char *filename, struct stat *statbuf) {
  return syscall_arg2(SYSCALL_STAT, (uint64_t)filename, (uint64_t)statbuf);
}

uint64_t syscall_brk(uint64_t brk) { return syscall_arg1(SYSCALL_BRK, brk); }
