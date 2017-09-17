/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "bg_jobs.h"
#include "process_group.h"
#include "yash_shell.h"

/*
 * Moves the foreground job to the background then clears the old, duplicate entry.
 * The new process group is inserted from the front of the list.
 */
void move_job_to_bg(process_group_t *process_group, bg_jobs_linked_list_t *linked_list)  {
    process_group->process_status = STOPPED;

    bg_jobs_linked_list_node_t *new_node = create_linked_list_node(process_group);

    if (linked_list->pointer_to_head->next) {
        // Stack is not empty
        new_node->next = linked_list->pointer_to_head->next;
        linked_list->pointer_to_head->next->previous = new_node;
    }
    else {
        // First node in linked_list
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

        yash->fg_job = most_recent_process_group_in_bg;
        yash->bg_jobs_linked_list->size--;
    } 
} 
