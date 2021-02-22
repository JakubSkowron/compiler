#!/bin/bash
set -e
CFLAGS="-Og -ggdb -ffreestanding -std=c17 -Wall --pedantic"
clang $CFLAGS -c exit.c
clang $CFLAGS -c syscall_64.c
ld -nostdlib -nolibc --entry=entry_point -o exit exit.o syscall_64.o
echo "Build done."

