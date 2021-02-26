#!/bin/bash
set -e
CC=clang
debug_flags="-O0 -ggdb"
release_flags="-O2"
CFLAGS="-ffreestanding -std=c17 -Wall --pedantic"
if [[ $1 == "--release" ]]; then
  CFLAGS="${release_flags} $CFLAGS"
else
  CFLAGS="${debug_flags} $CFLAGS"
fi
$CC $CFLAGS -c example.c
$CC $CFLAGS -c syscall_64.c
as -o entry_point.o entry_point.s
ld -static -nostdlib -nolibc --entry=entry_point -o example example.o syscall_64.o entry_point.o
echo "Build done."

