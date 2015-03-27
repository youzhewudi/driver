#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define PAGE_SIZE 4096

int main() {
  int fd = open("/dev/my_misc", O_RDWR);
  if (fd < 0) {
    printf("open file failed\n");
    return -1;
  }

  char *mem = (char*)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (mem == MAP_FAILED) {
    printf("mmap failed\n");
    return -1;
  }

  printf("msg from miscdev: %s\n", mem);
  munmap(mem, PAGE_SIZE);
  return 0;
}
