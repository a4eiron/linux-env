#include "ud_ucase.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

  struct sockaddr_un svaddr, claddr;
  int sfd, j;
  ssize_t numBytes;
  socklen_t len;
  char buf[BUF_SIZE];

  sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sfd == -1) {
    perror("socket");
    return 1;
  }

  if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
    perror("remove");
    return 1;
  }

  memset(&svaddr, 0, sizeof(struct sockaddr_un));
  svaddr.sun_family = AF_UNIX;
  strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

  if (bind(sfd, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_un))) {
    perror("bind");
    return 1;
  }

  for (;;) {
    len = sizeof(struct sockaddr_un);
    numBytes =
        recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *)&claddr, &len);

    if (numBytes == -1) {
      perror("recvfrom");
      return 1;
    }

    printf("server received %ld bytes from %s\n", (long)numBytes,
           claddr.sun_path);

    for (j = 0; j < numBytes; j++) {
      buf[j] = toupper((unsigned char)buf[j]);
    }

    if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *)&claddr, len) !=
        numBytes) {
      perror("sendto");
      exit(1);
    }
  }

  return 0;
}
