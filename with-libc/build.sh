#!/bin/bash
set -e
CC=clang
debug_flags="-O0 -ggdb"
release_flags="-O2"
CFLAGS="-std=c17 -Wall --pedantic -I.."
if [[ $1 == "--release" ]]; then
  CFLAGS="${release_flags} $CFLAGS"
else
  CFLAGS="${debug_flags} $CFLAGS"
fi

$CC $CFLAGS -ffreestanding -c c_entry_point.c
$CC $CFLAGS -ffreestanding -c ../syscall_64.c

as -o entry_point.o entry_point.s
$CC $CFLAGS -Wl,--entry=entry_point -o example main.c entry_point.o syscall_64.o c_entry_point.o
#if no --static ld wants example2.c to be compiled with -PIE in gcc (if -ffreestanding)
echo "Build done."

