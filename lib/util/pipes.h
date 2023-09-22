#pragma once

#include <stdbool.h>
#include <stddef.h>

int *pipes_create(size_t count);

void pipes_destroy(int *pipes, size_t count);

/**
 * @brief redirect the pipes. a 'task' should read its args from STDIN_FILENO and write to STDOUT_FILENO
 */
bool pipes_configure(int *pipes, size_t count, size_t pos);
