#include <stdbool.h>

typedef struct process_group_t {
    pid_t process_group_id;
    char *command;
    bool is_foreground_job;
} process_group_t;
