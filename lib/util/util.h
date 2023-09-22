#pragma once

#include <stdio.h>

/**
 * @brief split a c-string on whitespace 'in place'. i.e. modify `command` itself
 */
char **split(char *command);

/**
 * @brief consume and discard an entire line fromthe stream `stream`
 */
void discard_line(FILE *stream);
