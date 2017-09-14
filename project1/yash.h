#ifndef _YASH_H_
#define _YASH_H_

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdbool.h>

int status;
int pipefd[2];
char **parsed_input;
bool show_terminal_prompt;

static void sig_handler(int signum);

#endif
