#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <liburing.h>

#define QUEUE_DEPTH 4096

void usage(FILE *);
int kmemleak(struct io_uring *ring, long entries);

int main(int argc, char **argv) {
  struct io_uring ring;
  struct io_uring_sqe sqe;
  long entries;
  char *end;
  int err = 0;

  if (argc < 2) {
    fprintf(stderr, "error: number of entries to leak not specified\n");
    usage(stderr);
    return 1;
  }

  entries = strtol(argv[1], &end, 10);
  if (end == argv[1]) {
    fprintf(stderr, "failed to parse number of bytes to leak\n");
    usage(stderr);
    return 1;
  }

  // Setup ring.
  err = io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
  if (err != 0) {
    printf("io_uring init failed: %s\n", strerror(-err));
    return 1;
  }

  // Leak kernel memory.
  err = kmemleak(&ring, entries);
  if (err != 0) {
    printf("io_uring leak failed: %s\n", strerror(-err));
    return 1;
  }

  printf("memory leaked, sleeping...\n");

  // Sleep forever.
  for (;;)
    sleep(1);

  return 0;
}

void usage(FILE *f) {
  fprintf(f, "usage: kmemleak <number>\n");
  fprintf(f, "       kmemleak 1000\n");
}

int kmemleak(struct io_uring *ring, long entries) {
  struct io_uring_sqe *sqe;
  int err, s;

  // Submit tasks in loop.
  for (;;) {
    sqe = io_uring_get_sqe(ring);

    // Submission queue is full, submit it.
    if (sqe == NULL || entries <= 0) {
      s = io_uring_submit(ring);
      if (err != 0) {
        return -err;
      }

      if (s >= entries || entries <= 0)
        break;

      entries -= s;

      continue;
    }

    io_uring_prep_nop(sqe);

    // We never poll completions so kernel memory is never released.
  }

  return 0;
}
