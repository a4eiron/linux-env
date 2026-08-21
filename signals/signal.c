#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void handler(int sig) { printf("ouch\n"); }

static void handler2(int sig) {
  static int count = 0;

  if (sig == SIGINT) {
    count++;
    printf("caught SIGINT (%d)\n", count);
    return;
  }

  printf("caught SIGQUIT\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  if (signal(SIGINT, handler2) == SIG_ERR) {
    fprintf(stderr, "signal");
    return -1;
  }

  if (signal(SIGQUIT, handler2) == SIG_ERR) {
    fprintf(stderr, "signal");
    return -1;
  }

  for (;;) {
    pause();
  }
}
