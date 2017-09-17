#ifndef _BG_JOBS_H_
#define _BG_JOBS_H_ 

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "process_group.h"

#include <stdbool.h>

typedef struct bg_jobs_linked_list_node_t {
    struct process_group_t *process_group;
    struct bg_jobs_linked_list_node_t *previous;
    struct bg_jobs_linked_list_node_t *next;
    bool is_head_or_tail;
} bg_jobs_linked_list_node_t;

typedef struct bg_jobs_linked_list_t {
    size_t size;
    // These pointers act as boundaries for the linked_list.
    struct bg_jobs_linked_list_node_t *pointer_to_head; 
    struct bg_jobs_linked_list_node_t *pointer_to_tail;
} bg_jobs_linked_list_t; 

extern struct bg_jobs_linked_list_t linked_list;

bg_jobs_linked_list_t *create_bg_jobs_linked_list();
bg_jobs_linked_list_node_t *create_linked_list_node(process_group_t *process_group);
void remove_linked_list_node(process_group_t *process_group_to_remove, bg_jobs_linked_list_t *linked_list);

#endif
