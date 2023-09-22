#include "util.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static size_t count_spaces(char *text) {
  size_t count = 0;
  while (*text) {
    if (isspace(*text)) count++;
    text++;
  }
  return count;
}

char **split(char *command) {
  if (!command) return NULL;

  size_t const spaces_count = count_spaces(command);
  char **buf = malloc(sizeof *buf * (spaces_count + 2));  // one for the last word, another for NULL
  if (!buf) return NULL;

  size_t i = 0;
  for (char *t = strtok(command, " \t"); t; t = strtok(NULL, " \t")) {
    buf[i++] = t;

    if (i > spaces_count) break;
  }

  buf[i] = 0;
  return buf;
}

void discard_line(FILE *stream) {
  int c;
  do {
    c = fgetc(stream);
  } while (c != '\n');
}
