#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int fd, flags;
  char template[] = "/tmp/textXXXXXX";

  setbuf(stdout, NULL);

  fd = mkstemp(template);
  if (fd == -1) {
    fprintf(stderr, "mkstemp");
    return -1;
  }

  printf("File offset before fork(): %lld\n",
         (long long)lseek(fd, 0, SEEK_CUR));

  flags = fcntl(fd, F_GETFL);
  if (flags == -1) {
    fprintf(stderr, "fcntl - F_GETFL");
    return -1;
  }

  printf("O_APPEND flag before fork(): %s\n",
         (flags & O_APPEND) ? "on" : "off");

  switch (fork()) {
  case -1:
    fprintf(stderr, "fork");
    return -1;
  case 0:
    if (lseek(fd, 1000, SEEK_SET) == -1) {
      fprintf(stderr, "lseek");
      return -1;
    }

    flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
      fprintf(stderr, "fcntl - F_GETFL");
      return -1;
    }

    flags |= O_APPEND;
    if (fcntl(fd, F_SETFL, flags) == -1) {
      fprintf(stderr, "fcntl - F_SETFL");
      return -1;
    }
    exit(0);

  default:
    if (wait(NULL) == -1) {
      fprintf(stderr, "wait");
      return -1;
    }
    printf("child exited\n");

    printf("File offset after fork() in parent: %lld\n",
           (long long)lseek(fd, 0, SEEK_CUR));

    flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
      fprintf(stderr, "fcntl - F_GETFL");
      return -1;
    }

    printf("O_APPEND flag in parent after fork(): %s\n",
           (flags & O_APPEND) ? "on" : "off");
    exit(0);
  }
}
