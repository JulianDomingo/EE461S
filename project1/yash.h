#ifndef _YASH_H_
#define _YASH_H_

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "bg_jobs.h"
#include "process_group.h"

#include <stdbool.h>

int status;
int pipefd[2];
char **parsed_input;
bool show_terminal_prompt;
pid_t process_group_id;
pid_t pid_child1;
pid_t pid_child2;

typedef struct yash_t {
    pid_t process_id;
    struct bg_jobs_stack_t *bg_jobs_stack;
    struct process_group_t *active_process_group; 
    struct process_group_t *fg_job;
} yash_t;

static void sig_handler(int signum);
// The method below is stored in yash.c to avoid circular dependencies between yash.h and bg_jobs.h.
void move_job_to_fg(yash_t *yash);

#endif
