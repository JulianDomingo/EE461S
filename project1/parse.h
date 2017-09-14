#ifndef _PARSE_H_
#define _PARSE_H_ 

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "yash_shell.h"

#include <stdbool.h>

static const int MAX_CHARACTER_LIMIT = 2000;

bool parse_input(char *shell_input, yash_shell_t *yash);
char *trim(char *untrimmed);

#endif
