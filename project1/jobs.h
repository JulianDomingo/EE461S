struct bg_jobs_linked_list_t bg_jobs;

typedef struct bg_jobs_linked_list_t {
    size_t size;
    struct process_group_t head;
    struct process_group_t next;
} bg_jobs_linked_list_t;
