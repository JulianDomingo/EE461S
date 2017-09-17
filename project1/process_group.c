/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "process_group.h"
#include "command.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static const int max_commands_limit = 2;

/*
 * Deallocates the passed process_group_t variable.
 */
void destroy_process_group(process_group_t *process_group) {
    // free command strings
    command_t **commands = process_group->commands;
    for (int command = 0; command < process_group->commands_size; command++) {
        destroy_command(commands[command]); 
    } 

    // free command holder 
    free(process_group->commands);

    // free full command name 
    free(process_group->full_command);
}

/*
 * Returns a heap-allocated process_group_t variable.
 */
process_group_t *create_process_group(char *full_command) {
    process_group_t *process_group = malloc(sizeof(process_group_t));
    
    process_group->process_group_id = 0;
    
    process_group->process_status = INITIALIZING; 

    process_group->full_command = strdup(full_command);

    process_group->commands = malloc(sizeof(char *) * max_commands_limit); 
    process_group->commands_size = 0;

    // The process group is automatically foreground unless specified by '&' or explicitly 
    // placed into the background (which are both handled in the parse.c file).
    process_group->is_foreground_job = true;

    return process_group;
}

/*
 * Inserts a new command to the passed process group. 
 */
void add_new_command_to_process_group(process_group_t *process_group, command_t *command) {
    if (process_group->commands_size == 2) {
        perror("command capacity exceeded");
        exit(EXIT_FAILURE);
    }

    process_group->commands[process_group->commands_size] = command; 
    process_group->commands_size++;
}
