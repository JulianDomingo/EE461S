#ifndef _BG_JOBS_H_
#define _BG_JOBS_H_ 

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */
#include "process_group.h"

typedef struct bg_jobs_stack_node_t {
    struct process_group_t *process_group;
    struct process_group_t *previous;
    struct process_group_t *next;
} bg_jobs_stack_node_t;

typedef struct bg_jobs_stack_t {
    size_t size;
    struct bg_jobs_stack_node_t *head; 
} bg_jobs_stack_t; 

struct bg_jobs_stack_t stack;

void move_job_to_bg(yash_t *yash);

#endif
