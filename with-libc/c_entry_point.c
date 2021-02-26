#include "syscall_64.h"

size_t my_strlen(const char *str) {
  const char *p = str;
  while (*p++)
    ;
  return p - str;
}

int32_t my_print(const char *txt) {
  size_t len = my_strlen(txt);
  int64_t ret = syscall_write(SYS_STDOUT, txt, len);
  return ret;
}

void c_entry_point(uint64_t *entry_stack) { my_print("Before libc!\n"); }

void c_exit_point() { my_print("After. Called from libc::in __run_exit_handlers\n"); }
