#include <stdio.h>
#include <stdlib.h>

int foo(int *p) {
  printf("x = %d\n", *p);

  return *p;
}

int main(int argc, char **argv) {
  int *p = malloc(sizeof(int));

  foo(p);

  exit(EXIT_SUCCESS);
}
