#ifndef _PROCESS_GROUP_H_
#define _PROCESS_GROUP_H_

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdbool.h>

typedef struct process_group_t {
    pid_t process_group_id;
    int number_of_commands;
    char *command_entered;
    char **commands;
} process_group_t;

void initialize_process_group(process_group_t *process_group, char *command_string);
void destroy_process_group(process_group_t *process_group, bool is_foreground_job);
void allocate_new_command(process_group_t *process_group, char *command);

#endif
