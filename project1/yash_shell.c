/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "bg_jobs.h"
#include "process_group.h"
#include "yash_shell.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * Creates a new yash shell.
 */
yash_shell_t *create_yash_shell() {
    yash_shell_t *yash = malloc(sizeof(yash_shell_t));

    yash->process_id = getpid();
    yash->active_process_group = NULL;
    yash->fg_job = NULL;
    yash->bg_jobs_linked_list = create_bg_jobs_linked_list(); 

    return yash;
}

/*
 * Moves the foreground job to the background then clears the old, duplicate entry.
 * The new process group is inserted from the front of the list.
 */
void move_job_to_bg(process_group_t *process_group, bg_jobs_linked_list_t *linked_list)  {
    // Wrap process group in a node structure.
    bg_jobs_linked_list_node_t *new_node = create_linked_list_node(process_group);

    if (linked_list->pointer_to_head->next) {
        // Stack is not empty
        new_node->next = linked_list->pointer_to_head->next;
        linked_list->pointer_to_head->next->previous = new_node;
    }
    else {
        // Stack is empty 
        linked_list->pointer_to_tail->previous = new_node;  
        new_node->next = linked_list->pointer_to_tail;
    }    

    // These pointer re-routes must always happen regardless of the linked_list size. 
    new_node->previous = linked_list->pointer_to_head;
    linked_list->pointer_to_head->next = new_node;
    
    linked_list->size++;
}

/*
 * Moves the most recently inserted job in the background to the foreground, then
 * removes the old, duplicate entry from the linked_list.
 */
void move_job_to_fg(yash_shell_t *yash) {
    if (yash->bg_jobs_linked_list->size > 0) {
        // Move the background job at the top of the linked_list to the foreground.
        process_group_t *most_recent_process_group_in_bg = yash->bg_jobs_linked_list->pointer_to_head->next->process_group; 
        remove_linked_list_node(most_recent_process_group_in_bg, yash->bg_jobs_linked_list); 

        // If the most recent background job is stopped, ensure its status is set to running.
        if (most_recent_process_group_in_bg->process_status != RUNNING) {
            most_recent_process_group_in_bg->process_status = RUNNING;
        }

        yash->fg_job = most_recent_process_group_in_bg;

        yash->bg_jobs_linked_list->size--;
    } 
} 
