// private file mapping
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2 || strcmp(argv[1], "--help") == 0) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    return -1;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    perror("fstat");
    close(fd);
    return 1;
  }

  char *addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (addr == MAP_FAILED) {
    perror("mmap");
    return 1;
  }
  ssize_t n = write(STDOUT_FILENO, addr, sb.st_size);
  if (n == -1) {
    perror("write");
    close(fd);
    return 1;
  }

  if (n != sb.st_size) {
    fprintf(stderr, "partial write: %zd of %zu bytes\n", n, (size_t)sb.st_size);
    munmap(addr, sb.st_size);
    close(fd);
    return 1;
  }

  if (munmap(addr, sb.st_size) == -1) {
    perror("munmap");
    close(fd);
    return 1;
  }

  if (close(fd) == -1) {
    perror("close");
    return 1;
  }

  return 0;
}
