#ifndef _YASH_SHELL_H_
#define _YASH_SHELL_H_

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "process_group.h"
#include "bg_jobs.h"

typedef struct yash_shell_t {
    pid_t process_id;
    struct bg_jobs_stack_t *bg_jobs_stack;
    struct process_group_t *active_process_group; 
    struct process_group_t *fg_job;
} yash_shell_t;

void move_job_to_bg(process_group_t *process_group, bg_jobs_stack_t *stack);
void move_job_to_fg(yash_shell_t *yash);

#endif