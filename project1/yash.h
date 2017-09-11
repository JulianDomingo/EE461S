#ifndef _YASH_H_
#define _YASH_H_

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdbool.h>

#include "bg_jobs.h"
#include "process_group.h"

extern int status;
extern int pipefd[2];
extern char **parsed_input;
extern bool show_terminal_prompt;
extern pid_t process_group_id;
extern pid_t pid_child1;
extern pid_t pid_child2;

typedef struct yash_t {
    pid_t process_id;
    struct bg_jobs_linked_list_t *bg_jobs_list;
    struct process_group_t *active_process_group; 
    struct process_group_t *fg_job;
} yash_t;

void sig_handler(int signum);

#endif
