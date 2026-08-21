#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int cpid = fork();
  if (cpid == -1) {
    fprintf(stderr, "fork");
    return -1;
  } else {
    printf("cpid: %d\n", cpid);
  }

  switch (cpid) {
  case 0: {
    printf("child pid: %d\n", getpid());
    char *args[] = {"/bin/ls", "-la", NULL};
    execve("/bin/ls", args, NULL);
    perror("exeve");
    _exit(1);
  }
  default: {
    printf("parent pid: %d\n", getpid());
    int status;
    wait(&status);
  }
  }
  return 0;
}
