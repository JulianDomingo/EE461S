#ifndef _PROCESS_GROUP_H_
#define _PROCESS_GROUP_H_

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "command.h"

#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

enum status {RUNNING, STOPPED, INITIALIZING}; 

typedef struct process_group_t {
    pid_t process_group_id;
    char *full_command;
    struct command_t **commands; 
    size_t commands_size;
    enum status process_status; 
    bool is_foreground_job;
} process_group_t;

void initialize_process_group(process_group_t *process_group, char *command_string);
void destroy_process_group(process_group_t *process_group); 
void add_new_command_to_process_group(process_group_t *process_group, command_t *command);

#endif
