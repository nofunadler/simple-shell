#include "pipes.h"

#include <stdlib.h>
#include <unistd.h>

enum {
  READ_END = 0,
  WRITE_END,
  COUNT_END,
};

int *pipes_create(size_t count) {
  if (!count) return NULL;

  int *pipes = malloc(sizeof *pipes * count * COUNT_END);
  if (!pipes) return NULL;

  for (size_t i = 0; i < count; i++) {
    if (pipe(pipes + (i * COUNT_END)) == -1) {
      pipes_destroy(pipes, i);
      return NULL;
    }
  }

  return pipes;
}

void pipes_destroy(int *pipes, size_t count) {
  if (!pipes) return;

  for (size_t i = 0; i < count; i++) {
    close(pipes[i * COUNT_END + READ_END]);
    close(pipes[i * COUNT_END + WRITE_END]);
  }

  free(pipes);
}

bool pipes_configure(int *pipes, size_t count, size_t pos) {
  if (!pipes || !count) return false;

  if (pos == 0) {
    return dup2(pipes[pos * COUNT_END + WRITE_END], STDOUT_FILENO) != -1;
  } else if (pos == count) {
    return dup2(pipes[(pos - 1) * COUNT_END + READ_END], STDIN_FILENO) != -1;
  } else {
    return dup2(pipes[(pos - 1) * COUNT_END + READ_END], STDIN_FILENO) != -1 &&
           dup2(pipes[pos * COUNT_END + WRITE_END], STDOUT_FILENO) != -1;
  }
}
