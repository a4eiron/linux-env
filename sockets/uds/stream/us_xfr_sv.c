#include "us_xfr.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 5

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  int sfd, cfd;
  ssize_t numRead;

  char buf[BUF_SIZE];

  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1) {
    perror("socket");
    return 1;
  }

  if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
    perror("remove");
    return 1;
  }

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
    perror("bind");
    return 1;
  }

  if (listen(sfd, BACKLOG) == -1) {
    perror("listen");
    return 1;
  }

  for (;;) {

    cfd = accept(sfd, NULL, NULL);
    if (cfd == -1) {
      perror("accept");
      return 1;
    }

    while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
      if (write(STDOUT_FILENO, buf, numRead) != numRead) {
        perror("write");
        exit(1);
      }
    }

    if (numRead == -1) {
      perror("read");
      return 1;
    }

    if (close(cfd) == -1) {
      perror("close");
      return 1;
    }
  }

  return 0;
}
