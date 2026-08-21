#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sig_handler(int signum, siginfo_t *info, void *context) {
  printf("received signal: %d\n", signum);
  printf("signal code: %d\n", info->si_code);
  printf("sending process: %d\n", info->si_pid);
  if (signum == SIGINT) {
    printf("exiting gracefully");
    exit(0);
  }
}

int main(int argc, char *argv[]) {

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));

  sa.sa_sigaction = sig_handler;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);

  printf("process: %d\n", getpid());
  while (1) {
    printf("running..\n");
    sleep(1);
  }

  return 0;
}
