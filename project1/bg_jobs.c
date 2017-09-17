/*
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "bg_jobs.h"
#include "process_group.h"

#include <stdbool.h>
#include <stdlib.h>

/*
 * Removes the specified process group from the background jobs stack.
 * This function does nothing if the specified process group cannot be identified.
 */
void remove_stack_node(process_group_t *process_group_to_remove, bg_jobs_stack_t *stack) {
    if (stack->size == 0) { return; }

    bg_jobs_stack_node_t *stack_node_to_destroy;

    // Process group to remove is the most recent background job
    if (process_group_to_remove == stack->pointer_to_head->next->process_group) {
        // Save the pointer to the node to destroy before rerouting pointers.
        stack_node_to_destroy = stack->pointer_to_head->next;

        // Remove any pointers to the node to destroy.
        stack->pointer_to_head->next = stack->pointer_to_head->next->next;
        stack->pointer_to_head->next->next->previous = stack->pointer_to_head;

        // First free the heap-stored members of process group before freeing stack node itself.
        destroy_process_group(stack_node_to_destroy->process_group);
        free(stack_node_to_destroy); 
    }
    else {
        // Process group to remove is not the most recent background job
        bg_jobs_stack_node_t *runner = stack->pointer_to_head->next;
        while (!runner->is_head_or_tail) {
            if (runner->process_group == process_group_to_remove) {
                // Save the pointer to the node to destroy before rerouting pointers.
                stack_node_to_destroy = runner;

                // Remove any pointers to the node to destroy.
                runner->previous->next = runner->next; 
                runner->next->previous = runner->previous;

                // First free the heap-stored members of process group before freeing stack node itself.
                destroy_process_group(stack_node_to_destroy->process_group);
                free(stack_node_to_destroy); 
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
    stack_node->is_head_or_tail = false;
    return stack_node;
}

/*
 * Initializes the passed background jobs stack.
 */
void initialize_bg_jobs_stack(bg_jobs_stack_t *bg_jobs_stack) {
    bg_jobs_stack->size = 0;
    bg_jobs_stack->pointer_to_head = malloc(sizeof(bg_jobs_stack_node_t));
    bg_jobs_stack->pointer_to_tail = malloc(sizeof(bg_jobs_stack_node_t));
    bg_jobs_stack->pointer_to_head->is_head_or_tail = true;
    bg_jobs_stack->pointer_to_tail->is_head_or_tail = true;
    bg_jobs_stack->pointer_to_head->next = NULL;
}
