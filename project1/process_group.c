/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

#include "process_group.h"

static const int max_commands_limit = 2;

void destroy_process_group(process_group_t *process_group) {
    // free command strings
    for (int command = 0; command < max_commands_limit; command++) {
        free(process_group->commands[command]);
    } 

    // free command holder 
    free(process_group->commands);

    // free full command name 
    free(process_group->command_entered);
}

void initialize_process_group(process_group_t *process_group, char *command_entered) {
    process_group->command_entered = strdup(command_entered);
    process_group->commands = malloc(sizeof(char *) * max_commands_limit); 
    process_group->process_status = INITIALIZING; 
}

void allocate_new_command(process_group_t *process_group, char *command) {
    if (process_group->command_size == 2) {
        perror("command capacity exceeded");
        exit(EXIT_FAILURE);
    }
    process_group->commands[process_group->command_size] = strdup(command); 
    process_group->command_size++;
}
