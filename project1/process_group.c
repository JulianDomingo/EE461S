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

void destroy_process_group(process_group_t *process_group) {
    // free command strings
    for (int command = 0; command < process_group->commands_size; command++) {
        destroy_command(process_group->commands[command]); 
    } 

    // free command holder 
    free(process_group->commands);

    // free full command name 
    free(process_group->full_command);
}

void initialize_process_group(process_group_t *process_group, char *full_command) {
    process_group->full_command = strdup(full_command);
    process_group->commands = malloc(sizeof(char *) * max_commands_limit); 
    process_group->process_status = INITIALIZING; 
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
