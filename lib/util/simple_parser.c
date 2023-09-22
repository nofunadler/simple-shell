#include "simple_parser.h"

#include <stdbool.h>
#include <stdlib.h>

#define PIPE '|'
#define INIT_CAPACITY 10
#define GROWTH_FACTOR 2

static bool cmd_resize(struct commands **cmd) {
  if (!cmd || !*cmd) return false;

  struct commands *tmp = realloc(*cmd, sizeof *tmp + sizeof *tmp->commands * (*cmd)->capacity * GROWTH_FACTOR);
  if (!tmp) return false;

  tmp->capacity *= GROWTH_FACTOR;
  *cmd = tmp;
  return true;
}

static struct commands *cmd_create(void) {
  struct commands *cmd = malloc(sizeof *cmd + INIT_CAPACITY * sizeof *cmd->commands);
  if (!cmd) return NULL;

  *cmd = (struct commands){.capacity = INIT_CAPACITY};  // the rest are empty init
  return cmd;
}

static bool cmd_push(struct commands **cmd, char *ptr) {
  if (!cmd || !*cmd) return false;

  if ((*cmd)->count == (*cmd)->capacity) {
    if (!cmd_resize(cmd)) return false;
  }

  (*cmd)->commands[(*cmd)->count++] = ptr;
  return true;
}

struct commands *sp_parse(char *input, size_t size) {
  if (!input || !size) return NULL;

  struct commands *cmd = cmd_create();
  if (!cmd) return NULL;

  char *runner = input;
  while (true) {
    if (*runner == PIPE) {
      *runner = 0;
      if (!cmd_push(&cmd, input)) {
        sp_commands_destroy(cmd);
        return NULL;
      }

      input = runner + 1;
    }

    runner++;

    if (!*runner) {
      if (!cmd_push(&cmd, input)) {
        sp_commands_destroy(cmd);
        return NULL;
      }
      break;
    }
  }

  return cmd;
}

void sp_commands_destroy(struct commands *commands) {
  if (!commands) return;

  free(commands);
}
