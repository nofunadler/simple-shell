#pragma once

#include <stddef.h>

struct commands {
  size_t capacity;
  size_t count;
  char *commands[];
};

/**
 * @brief parses `input` to a `commands`. split the text on a pipe char ('|')
 *
 * @param input
 * @param len the length of input
 * @return struct cmd_vec
 */
struct commands *sp_parse(char *input, size_t len);

void sp_commands_destroy(struct commands *commands);
