#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  sleep(10);

  if (argc != 2) {
    printf("memory leak size argument is missing\n");
    return 1;
  }

  char *end = NULL;
  long to_leak = strtol(argv[1], &end, 10);
  if (end == argv[1]) {
    printf("failed to parse number of bytes to leak\n");
    return 1;
  }

  for (;;) {
    size_t bytes = to_leak % 4096 == 0 ? 4096 : to_leak % 4096;
    void *ptr = malloc(bytes);
    to_leak -= bytes;
    if (ptr == NULL) {
      printf("malloc failed\n");
      return 1;
    }

    if (to_leak <= 0)
      break;
  }

  printf("memory leaked\n");
  for (;;)
    sleep(1);

  return 0;
}
