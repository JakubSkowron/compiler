#!/bin/bash
set -e
debug_flags="-O0 -ggdb"
release_flags="-O2"
CFLAGS="-ffreestanding -std=c17 -Wall --pedantic"
if [[ $1 == "--debug" ]]; then
  CFLAGS="${debug_flags} $CFLAGS"
else
  CFLAGS="${release_flags} $CFLAGS"
fi
clang $CFLAGS -c example.c
clang $CFLAGS -c syscall_64.c
as -o entry_point.o entry_point.s
ld -nostdlib -nolibc --entry=entry_point -o example example.o syscall_64.o entry_point.o
echo "Build done."

