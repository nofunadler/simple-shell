#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pipes.h"
#include "simple_parser.h"
#include "util.h"

#define PROMPT "sh$ "
#define EXIT "exit"

void stream_discard_line(FILE *stream) {
  int c;
  do {
    c = fgetc(stream);
  } while (c != '\n');
}

void launch_processes(struct commands *cmd, int *pipes, size_t pipes_count) {
  for (size_t i = 0; i < cmd->count; i++) {
    pid_t pid = fork();

    switch (pid) {
      case -1:
        fprintf(stderr, "fork: %s\n", strerror(errno));
        break;
      case 0: {
        pipes_configure(pipes, pipes_count, i);
        pipes_destroy(pipes, pipes_count);

        char **_argv = split(cmd->commands[i]);
        if (!_argv) return;

        if (execvp(_argv[0], _argv) != 0) {
          fprintf(stderr, "execvp: '%s' unrecognizable command. %s\n", _argv[0], strerror(errno));

          free(_argv);
          return;
        }
      } break;
      default:
        break;
    }
  }
}

int main(void) {
  enum { BUF_SIZE = 4096 };
  char buf[BUF_SIZE];

  // main event loop
  while (true) {
    fprintf(stdout, "%s", PROMPT);
    fflush(stdout);

    if (!fgets(buf, sizeof buf, stdin)) {
      fprintf(stderr, "I/O error: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

    char *endline = strchr(buf, '\n');
    if (!endline) {
      fprintf(stderr, "invalid command: long argument list\n");
      stream_discard_line(stdin);
      continue;
    }
    *endline = 0;

    if (strcmp(buf, EXIT) == 0) break;

    struct commands *cmd = sp_parse(buf, sizeof buf);
    if (!cmd) {
      fprintf(stderr, "invalid command: parser error\n");
      continue;
    }

    size_t const pipes_count = cmd->count ? cmd->count - 1 : 0;
    int *pipes = pipes_create(pipes_count);  // may return NULL. handled on each usage of pipes

    launch_processes(cmd, pipes, pipes_count);

    size_t const cmd_count = cmd->count;
    sp_commands_destroy(cmd);

    pipes_destroy(pipes, pipes_count);

    size_t terminated_count = 0;
    do {
      int status;
      (void)wait(&status);

      if (WIFEXITED(status)) terminated_count++;
    } while (terminated_count < cmd_count);
  }

  return EXIT_SUCCESS;
}
