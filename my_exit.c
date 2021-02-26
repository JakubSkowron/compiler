#define SYSCALL_EXIT 60
typedef unsigned long uint64_t;

__attribute__((noreturn)) void c_entry_point(uint64_t *entry_stack, void (*finalizer)(void)) {
  __asm__ volatile("syscall \n"
                   : /* no output */
                   : /* rax */ "a"(SYSCALL_EXIT),
                     /* rdi */ "D"(5));
  __builtin_unreachable();
}