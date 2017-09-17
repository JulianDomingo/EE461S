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
void move_job_to_bg(process_group_t *process_group, bg_jobs_stack_t *stack)  {
    process_group->process_status = STOPPED;

    bg_jobs_stack_node_t *new_node = create_stack_node(process_group);

    if (stack->pointer_to_head->next) {
        // Stack is not empty
        new_node->next = stack->pointer_to_head->next;
        stack->pointer_to_head->next->previous = new_node;
    }
    else {
        // First node in stack
        stack->pointer_to_tail->previous = new_node;  
        new_node->next = stack->pointer_to_tail;
    }    

    // These pointer re-routes must always happen regardless of the stack size. 
    new_node->previous = stack->pointer_to_head;
    stack->pointer_to_head->next = new_node;
    
    stack->size++;
}

/*
 * Moves the most recently inserted job in the background to the foreground, then
 * removes the old, duplicate entry from the stack.
 */
void move_job_to_fg(yash_shell_t *yash) {
    if (yash->bg_jobs_stack->size > 0) {
        // Move the background job at the top of the stack to the foreground.
        process_group_t *most_recent_process_group_in_bg = yash->bg_jobs_stack->pointer_to_head->next->process_group; 
        remove_stack_node(most_recent_process_group_in_bg, yash->bg_jobs_stack); 

        yash->fg_job = most_recent_process_group_in_bg;
        yash->bg_jobs_stack->size--;
    } 
} 
