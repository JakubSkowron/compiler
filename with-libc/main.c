#include <stdio.h>

int main(int argc, const char **argv) {
  while (*argv) {
    printf("%s\t", *argv);
    ++argv;
  }
  puts("");
  return 0;
}
