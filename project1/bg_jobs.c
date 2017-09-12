/*
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdlib.h>

#include "process_group.h"
#include "bg_jobs.h"

/*
 * Removes the specified process group from the background jobs stack.
 * This function does nothing if the specified process group cannot be identified.
 */
void remove_stack_node(process_group_t *process_group_to_remove, bg_jobs_stack_t *stack) {
    if (stack->size == 0) { return; }

    if (process_group_to_remove == stack->head->process_group) {
        // First free the heap-stored members of the process group before freeing the stack node itself.
        destroy_process_group(stack->head->process_group);
        free(stack->head);
        stack->head->process_group = stack->head->process_group->next;    
    }
    else {
        bg_jobs_stack_node_t *runner = stack->head;
        while (runner->next != NULL) {
            if (runner->process_group == process_group_to_remove) {
                // Re-route pointers
                destroy_process_group(runner->process_group);
                runner->previous->next = runner->next;
                runner->next->previous = runner->previous;

                // Nullify unused pointers.
                runner->previous = NULL;
                runner->next = NULL;

                // Deallocate space on heap for removed stack node. 
                free(runner);
            }

            runner = runner->next;
        } 
    }
}

/*
 * Returns a new bg job stack node.
 */
bg_jobs_stack_node_t *create_stack_node(process_group_t *process_group) {
    bg_jobs_stack_node_t *stack_node = malloc(sizeof(bg_jobs_stack_node_t));
    stack_node->process_group = process_group;
    stack_node->next = NULL;
    stack_node->previous = NULL;
    return stack_node;
}

/*
 * Moves the foreground job to the background then clears the old, duplicate entry.
 */
void move_job_to_bg(process_group_t *process_group, bg_jobs_stack_t *stack)  {
    process_group->process_status = STOPPED;

    bg_jobs_stack_node_t *new_node = create_stack_node(process_group);

    if (stack->head) {
        // Head process becomes second most recent
        new_node->next = stack->head;
        stack->head->previous = new_node;
    }
    
    stack->head = new_node;
    
    stack->size++;
}

/*
 * Moves the most recently inserted job in the background to the foreground.
 * If no background job exists, prints the indication to STDOUT as does bash. 
 */
void move_job_to_fg(yash_t *yash) {
    if (yash->fg_job) {
        bg_jobs_stack_node_t *most_recent_job    
                



    } 
    else {
        printf("fg: no current job");
    }
} 
