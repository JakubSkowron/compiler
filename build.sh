cc -O2 -ggdb -fno-builtin -nostdlib -c exit.c
ld -nostdlib -nolibc --entry=entry_point -o exit exit.o

