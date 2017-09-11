#ifndef _BG_JOBS_H_
#define _BG_JOBS_H_ 

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */
#include "process_group.h"

typedef struct bg_jobs_linked_list_t {
    size_t size;
    struct process_group_t *head;
    struct process_group_t *next;
} bg_jobs_linked_list_t;

struct bg_jobs_linked_list_t head; 

#endif
