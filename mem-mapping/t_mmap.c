// shared file mapping
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEM_SIZE 10

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "--help") == 0) {
    fprintf(stderr, "Usage: %s file [new-value]\n", argv[0]);
    return 1;
  }

  int fd = open(argv[1], O_RDWR);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  char *addr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    perror("mmap");
    close(fd);
    return 1;
  }

  if (close(fd) == -1) {
    perror("close");
    return 1;
  }

  printf("current string: %.*s\n", MEM_SIZE, addr);
  if (argc > 2) {
    if (strlen(argv[2]) >= MEM_SIZE) {
      fprintf(stderr, "'new-value' too large\n");
      munmap(addr, MEM_SIZE);
      return 1;
    }

    memset(addr, 0, MEM_SIZE);
    strncpy(addr, argv[2], MEM_SIZE - 1);
    if (msync(addr, MEM_SIZE, MS_SYNC) == -1) {
      perror("msync");
      return 1;
    }
    printf("copied \"%s\" to shared memory\n", argv[2]);
  }
  if (munmap(addr, MEM_SIZE) == -1) {
    perror("munmap");
    return 1;
  }

  return 0;
}
