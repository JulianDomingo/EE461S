/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

#include "process_group.h"

#define MAX_COMMANDS_LIMIT 2;

void destroy_process_group(process_group_t *process_group) {
    // free command strings
    for (int command = 0; command < process_group->MAX_COMMANDS_LIMIT; command++) {
        free(process_group->commands[command]);
    } 

    // free command holder 
    free(process_group->commands);

    // free full command name 
    free(process_group->command_entered);
}

void initialize_process_group(process_group_t *process_group, char *command_entered) {
    process_group->command_entered = strdup(command_entered);
    process_group->MAX_COMMANDS_LIMIT = MAX_COMMANDS_LIMIT;
    process_group->command_entered = strdup(command_entered); 
    process_group->commands = malloc(sizeof(char *) * MAX_COMMANDS_LIMIT); 
    process_group->is_running = INITIALIZING; 
}

void allocate_new_command(process_group_t *process_group, char *command) {
    process_group->commands[process_group->command_size] = strdup(command); 
    process_group->command_size++;
}
