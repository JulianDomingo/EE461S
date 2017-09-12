#ifndef _PROCESS_GROUP_H_
#define _PROCESS_GROUP_H_

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

enum status {RUNNING, STOPPED, INITIALIZING}; 

typedef struct process_group_t {
    pid_t process_group_id;
    char *command_entered;
    char **commands;
    size_t command_size;
    enum status;  
} process_group_t;

void initialize_process_group(process_group_t *process_group, char *command_string);
void destroy_process_group(process_group_t *process_group); 
void allocate_new_command(process_group_t *process_group, char *command);

#endif
