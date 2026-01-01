#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PAGE_SIZE 4096

void usage(FILE *);
static inline int memleak(long pages);

int main(int argc, char **argv) {
  char *end;
  long pages;
  int err;

  if (argc != 2) {
    fprintf(stderr, "error: number of page to leak not specified\n");
    usage(stderr);
    return 1;
  }

  pages = strtol(argv[1], &end, 10);
  if (end == argv[1]) {
    fprintf(stderr, "failed to parse number of bytes to leak\n");
    usage(stderr);
    return 1;
  }

  // Leak memory.
  err = memleak(pages);
  if (err != 0)
    return err;

  printf("memory leaked, sleeping...\n");

  // Sleep forever.
  for (;;)
    sleep(1);

  return 0;
}

void usage(FILE *f) {
  fprintf(f, "usage: memleak <pages>\n");
  fprintf(f, "       memleak 1000\n");
}

static inline int memleak(long pages) {
  void *ptr;

  for (;;) {
    ptr = malloc(PAGE_SIZE);
    pages -= 1;
    if (ptr == NULL)
      return -ENOMEM;

    if (pages <= 0)
      break;
  }

  return 0;
}
