#ifndef _YASH_H_
#define _YASH_H_

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdbool.h>

extern int status;
extern int pipefd[2];
extern char **parsed_input;
extern bool show_terminal_prompt;

static void sig_handler(int signum);

#endif
