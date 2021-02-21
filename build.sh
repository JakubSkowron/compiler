clang -O2 -ggdb -fno-builtin -nostdlib -std=c17 -Wall --pedantic -c exit.c
ld -nostdlib -nolibc --entry=entry_point -o exit exit.o

