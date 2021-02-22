#ifndef SYSCALL_64_H
#define SYSCALL_64_H

typedef int int32_t;
typedef unsigned uint32_t;
typedef long int64_t;
typedef unsigned long uint64_t;
_Static_assert(sizeof(int32_t) == 4, "");
_Static_assert(sizeof(uint32_t) == 4, "");
_Static_assert(sizeof(int64_t) == 8, "");
_Static_assert(sizeof(uint64_t) == 8, "");
typedef uint64_t size_t;

/*
 * syscall numbers:
 * [ /usr/src/linux-headers/arch/x86/include/generated/asm/syscalls_64.h ]
 * [ https://github.com/torvalds/linux/blob/master/arch/x86/entry/syscalls/syscall_64.tbl ]
 * 
 * syscall headers:
 * [ https://github.com/torvalds/linux/blob/master/include/linux/syscalls.h ]
 */

#define SYSCALL_READ 0    // sys_read(unsigned int fd, char __user *buf, size_t count);
#define SYSCALL_WRITE 1   // sys_write(unsigned int fd, const char __user *buf, size_t count);
#define SYSCALL_OPEN 2    // sys_open(const char __user *filename, int flags, umode_t mode);
#define SYSCALL_CLOSE 3   // sys_close(unsigned int fd);
#define SYSCALL_STAT 4    // sys_newstat(const char __user *filename, struct stat __user *statbuf);
#define SYSCALL_FSTAT 5   // sys_fstat(unsigned int fd, struct __old_kernel_stat __user *statbuf);
#define SYSCALL_LSTAT 6   // sys_lstat(const char __user *filename, struct __old_kernel_stat __user *statbuf);
#define SYSCALL_MMAP 9    // ksys_mmap_pgoff(unsigned long addr, unsigned long len,
                       //                 unsigned long prot, unsigned long flags,
                       //                 unsigned long fd, unsigned long pgoff);
#define SYSCALL_MPROTECT 10  // sys_mprotect(unsigned long start, size_t len, unsigned long prot);
#define SYSCALL_MUNMAP 11    // sys_munmap(unsigned long addr, size_t len);
#define SYSCALL_BRK 12       // sys_brk(unsigned long brk);
#define SYSCALL_PREAD64 17   // sys_pread64(unsigned int fd, char __user *buf, size_t count, loff_t pos);
#define SYSCALL_PWRITE64 18  // sys_pwrite64(unsigned int fd, const char __user *buf, size_t count, loff_t pos);
#define SYSCALL_EXIT 60   // sys_exit(int error_code);

uint64_t syscall_arg1(uint64_t syscall_number, uint64_t arg1);
uint64_t syscall_arg2(uint64_t syscall_number, uint64_t arg1, uint64_t arg2);
uint64_t syscall_arg3(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3);
uint64_t syscall_arg4(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);
uint64_t syscall_arg5(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
                      uint64_t arg5);
uint64_t syscall_arg6(uint64_t syscall_number, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
                      uint64_t arg5, uint64_t arg6);

typedef int64_t __kernel_long_t;
typedef uint64_t __kernel_ulong_t;

// [ https://github.com/torvalds/linux/blob/master/arch/x86/include/uapi/asm/stat.h ]
struct stat {
   __kernel_ulong_t   st_dev;
   __kernel_ulong_t   st_ino;
   __kernel_ulong_t   st_nlink;

   unsigned int       st_mode;
   unsigned int       st_uid;
   unsigned int       st_gid;
   unsigned int       __pad0;
   __kernel_ulong_t   st_rdev;
   __kernel_long_t    st_size;
   __kernel_long_t    st_blksize;
   __kernel_long_t    st_blocks;   /* Number 512-byte blocks allocated. */

   __kernel_ulong_t   st_atime;
   __kernel_ulong_t   st_atime_nsec;
   __kernel_ulong_t   st_mtime;
   __kernel_ulong_t   st_mtime_nsec;
   __kernel_ulong_t   st_ctime;
   __kernel_ulong_t   st_ctime_nsec;
   __kernel_long_t    __unused[3];
};

__attribute__((noreturn)) void syscall_exit(uint32_t exit_code);
size_t syscall_read(uint32_t fd, char* buf, size_t count);
size_t syscall_write(uint32_t fd, const char* buf, size_t count);
uint64_t syscall_stat(const char* filename, struct stat* statbuf);
uint64_t syscall_brk(uint64_t brk);

#endif  // SYSCALL_64_H
