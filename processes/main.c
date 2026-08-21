// storage classes
// 1. register - stored in register if possible, otherwise stack; cannot take
// addresses;
// 2. static - lives for entire program execution; stored in data segment (not
// stack); preserves values b/w function calls;
// 		global statics - lives for entire program execution; scope
// limited to the file; stored in data segment;
// 3. extern - used for declaration; lives for the entire program; stored in
// data segment;

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

char globBuf[65536];         // uninitialized data seg
int primes[] = {2, 3, 5, 7}; // initialized data seg

static int square(int x) {
  int result; // in the frame of square
  result = x * x;
  return result; // passed via register
}

int main(int argc, char *argv[]) {

  clearenv();

  static int key = 10;    // initialized data seg
  static char mbuf[1024]; // uninitialized data seg
  char *p;                // in the frame of main

  p = malloc(1024); // points to memory on the heap

  printf("pid: %d\n", getpid());
  printf("parent pid: %d\n", getppid());

  for (char **p = argv; *p != NULL; p++) {
    puts(*p);
  }

  if (putenv("EDITOR=nvim") != 0) {
    fprintf(stderr, "putenv");
    return -1;
  }

  unsetenv("EDITOR");

  if (setenv("TERMINAL", "kitty", 1) != 0) {
    fprintf(stderr, "setenv");
    return -1;
  }

  for (char **ep = environ; *ep != NULL; ep++) {
    puts(*ep);
  }

  sleep(30);

  return 0;
}
