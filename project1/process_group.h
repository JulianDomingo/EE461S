#include <stdbool.h>

typedef struct process_group_t {
    pid_t process_group_id;
    char *command;
    char **commands;
} process_group_t;

void create_process_group(int pid, char *command);
void destroy_process_group(process_group_t *process_group);
void allocate_new_command(char ***command);
