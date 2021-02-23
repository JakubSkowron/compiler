/*
gcc -ffreestanding -std=c17 -c exit.c
ld -nostdlib -nolibc --entry=entry_point -o exit exit.o syscall_64.o
*/

#include "syscall_64.h"

#define SYS_STDIN  0
#define SYS_STDOUT 1
#define SYS_STDERR 2

size_t strlen(const char* str) {
  const char* p = str;
  while( *p++ );
  return p - str;
}

void assert(_Bool condition, const char *msg) {
  if (condition)
    return;
  syscall_arg3(SYSCALL_WRITE, SYS_STDERR, (uint64_t)msg, strlen(msg));
  syscall_exit(1);
}

int32_t print(const char *txt) {
  size_t len = strlen(txt);
  int64_t ret = syscall_arg3(SYSCALL_WRITE, SYS_STDOUT, (uint64_t)txt, len);
  assert(ret >= 0, "print failed\n");
  assert(ret == len, "print got unexpecded length\n");
  return ret;
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

__attribute__((noreturn)) void c_entry_point(uint64_t *entry_stack) {
  uint64_t argc = entry_stack[0];
  const char **argv = (const char **)(entry_stack + 1);
  const char **envp = argv + argc + 1; // 1 for argv terminating zero

  while(*argv) {
    print(*argv++);
    print("\t");
  }
  print("\n");

  while (*envp) {
    print(*envp++);
    print(";");
  }
  print("\n");

  uint64_t* rsp;
  uint64_t* rbp;
  __asm__(
    "movq %%rsp, %[rsp]\n"
    "movq %%rbp, %[rbp]"
    : [rsp] "=g" (rsp), [rbp] "=g" (rbp) : /* no input */: /* no clobber */
  );
  print("rsp, rbp:\n");
  print_hex((uint64_t)rsp);
  print_hex((uint64_t)rbp);

  const char* txt = "Hello, World!\n";
  syscall_arg3(SYSCALL_WRITE, SYS_STDOUT, (uint64_t)txt, strlen(txt));
  print("Testing syscall_brk\n");
  uint64_t mem = syscall_brk(0);
  print_hex(mem);
  uint64_t mem_end = syscall_brk(mem + 100);
  print_hex(mem_end);
  int *i = ((int *)mem) + 15;
  *i = 0xABCD1234;
  print_hex((uint64_t)i);
  print_hex((uint64_t)*i);

  print("Testing stat\n");

  struct stat statbuf;
  uint64_t status = syscall_stat("build.sh", &statbuf);
  assert(status == 0, "stat64 failed\n");
  print("File size is: ");
  print_hex(statbuf.st_size);

  syscall_exit(0);
}
