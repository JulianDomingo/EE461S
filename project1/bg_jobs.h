#ifndef _BG_JOBS_H_
#define _BG_JOBS_H_ 

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "process_group.h"
#include "yash.h"

typedef struct bg_jobs_stack_node_t {
    struct process_group_t *process_group;
    struct bg_jobs_stack_node_t *previous;
    struct bg_jobs_stack_node_t *next;
} bg_jobs_stack_node_t;

typedef struct bg_jobs_stack_t {
    size_t size;
    struct bg_jobs_stack_node_t *head; 
} bg_jobs_stack_t; 

struct bg_jobs_stack_t stack;

void remove_stack_node(process_group_t *process_group_to_remove, bg_jobs_stack_t *stack);
void move_job_to_bg(process_group_t *process_group, bg_jobs_stack_t *stack);
void move_job_to_fg(process_group_t *process_group);
bg_jobs_stack_node_t *create_stack_node(process_group_t *process_group);

#endif
