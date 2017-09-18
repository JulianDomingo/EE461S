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
    struct bg_jobs_linked_list_t *bg_jobs_linked_list;
    struct process_group_t *active_process_group; 
    struct process_group_t *fg_job;
} yash_shell_t;


yash_shell_t *create_yash_shell();
void move_job_to_fg(yash_shell_t *yash);
void move_job_to_bg(process_group_t *process_group, bg_jobs_linked_list_t *linked_list); 
void check_for_bg_job_status_updates(bg_jobs_linked_list_t *linked_list);

#endif
