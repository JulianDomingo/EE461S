#include "process_group.h"

typedef struct yash_t {
    pid_t process_id;
    struct bg_jobs_linked_list_t bg_jobs_list;
    struct process_group_t current_process_group; 
}
