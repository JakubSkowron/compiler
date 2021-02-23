#!/bin/bash
set -e
CFLAGS="-O2 -ggdb -ffreestanding -std=c17 -Wall --pedantic"
clang $CFLAGS -c example.c
clang $CFLAGS -c syscall_64.c
as -o entry_point.o entry_point.s
ld -nostdlib -nolibc --entry=entry_point -o example example.o syscall_64.o entry_point.o
echo "Build done."

