
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

void destroy_process_group(process_group_t *process_group, bool is_foreground_job) {
    if (is_foreground_job) {
        // TODO: Implement foreground job removal logic
    }
    else {
        // TODO: Implement background job removal logic
    }
}

void initialize_process_group(process_group_t *process_group, char *command_entered) {
    process_group->command_entered = strdup(command_entered);
    process_group->number_of_commands = MAX_COMMANDS_LIMIT;
    process_group->command_entered = strdup(command_entered); 
    process_group->commands = malloc(sizeof(char *) * MAX_COMMANDS_LIMIT); 
}

void allocate_new_command(process_group_t *process_group, char *command) {
    // TODO: Implement logic
}
