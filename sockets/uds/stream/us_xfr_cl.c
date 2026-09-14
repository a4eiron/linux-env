#include "us_xfr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  int sfd;
  ssize_t numRead;
  char buf[BUF_SIZE];

  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1) {
    perror("socket");
    return 1;
  }

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

  if (connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) ==
      -1) {
    perror("connect");
    return 1;
  }

  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
    if (write(sfd, buf, numRead) != numRead) {
      perror("write");
      exit(1);
    }
  }

  if (numRead == -1) {
    perror("read");
    return 1;
  }

  if (close(sfd) == -1) {
    perror("close");
    return 1;
  }
  return 0;
}
