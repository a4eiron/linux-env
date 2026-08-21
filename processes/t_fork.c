#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static int idata = 111;

static void chld_handler(int sig) {
  printf("something happened to my child. welp!!!!!\n");
}

int main(int argc, char *argv[]) {

  int istack = 222;

  // returns 0 in the child process and
  // the pid of child in the parent process

  pid_t child_pid = fork();

  if (child_pid == -1) {
    fprintf(stderr, "fork");
    return -1;
  }

  signal(SIGCHLD, chld_handler);

  if (child_pid == 0) {
    idata *= 3;
    istack *= 3;
  }
  printf("PID=%ld %s idata=%d istack=%d\n", (long)getpid(),
         (child_pid == 0 ? "(child)" : "(parent)"), idata, istack);

  if (wait(NULL) == -1) {
    fprintf(stderr, "wait");
    return -1;
  }

  return 0;
}
