/*
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "bg_jobs.h"
#include "process_group.h"

#include <stdbool.h>
#include <stdlib.h>

/*
 * Returns a new bg job linked_list node.
 */
bg_jobs_linked_list_node_t *create_linked_list_node(process_group_t *process_group) {
    bg_jobs_linked_list_node_t *linked_list_node = malloc(sizeof(bg_jobs_linked_list_node_t));

    linked_list_node->process_group = process_group;
    linked_list_node->next = NULL;
    linked_list_node->previous = NULL;
    linked_list_node->is_head_or_tail = false;

    return linked_list_node;
}

/*
 * Removes the specified process group from the background jobs linked_list.
 * This function does nothing if the specified process group cannot be identified, or if the linked list is empty.
 */
void remove_linked_list_node(process_group_t *process_group_to_remove, bg_jobs_linked_list_t *linked_list) {
    if (linked_list->size == 0) { return; }

    bg_jobs_linked_list_node_t *temp_pointer;

    // Process group to remove is the most recent background job
    if (process_group_to_remove == linked_list->pointer_to_head->next->process_group) {
        // Save the pointer to the node to destroy before rerouting pointers.
        temp_pointer = linked_list->pointer_to_head->next;

        // Remove any pointers to the node to destroy.
        linked_list->pointer_to_head->next = linked_list->pointer_to_head->next->next;
        linked_list->pointer_to_head->next->next->previous = linked_list->pointer_to_head;

        // First free the heap-stored members of process group before freeing linked_list node itself.
        destroy_process_group(temp_pointer->process_group);
        free(temp_pointer); 
    }
    else {
        // Process group to remove is not the most recent background job
        bg_jobs_linked_list_node_t *runner = linked_list->pointer_to_head->next;
        while (!runner->is_head_or_tail) {
            if (runner->process_group == process_group_to_remove) {
                // Save the pointer to the node to destroy before rerouting pointers.
                temp_pointer = runner;

                // Remove any pointers to the node to destroy.
                runner->previous->next = runner->next; 
                runner->next->previous = runner->previous;

                // First free the heap-stored members of process group before freeing linked_list node itself.
                destroy_process_group(temp_pointer->process_group);
                free(temp_pointer); 
            }

            runner = runner->next;
        } 
    }
}

/*
 * Initializes the passed background jobs linked_list.
 */
bg_jobs_linked_list_t *create_bg_jobs_linked_list() {
    bg_jobs_linked_list_t *bg_jobs_linked_list = malloc(sizeof(bg_jobs_linked_list_t));

    bg_jobs_linked_list->size = 0;
    bg_jobs_linked_list->pointer_to_head = malloc(sizeof(bg_jobs_linked_list_node_t));
    bg_jobs_linked_list->pointer_to_tail = malloc(sizeof(bg_jobs_linked_list_node_t));
    bg_jobs_linked_list->pointer_to_head->is_head_or_tail = true;
    bg_jobs_linked_list->pointer_to_tail->is_head_or_tail = true;
    bg_jobs_linked_list->pointer_to_head->next = NULL;
    bg_jobs_linked_list->pointer_to_head->previous = NULL;
}
