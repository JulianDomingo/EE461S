#ifndef _COMMAND_H_
#define _COMMAND_H_

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdlib.h>
#include <stdbool.h>

typedef struct command_t {
    // Process information
    pid_t process_group_id;

    // File redirection
    bool contains_redirect_stdin;
    bool contains_redirect_stdout;
    bool contains_redirect_stderr;
    char *redirect_stdin_filename;
    char *redirect_stdout_filename;
    char *redirect_stderr_filename;

    // Command information 
    char **whitespace_tokenized_command; 
    size_t whitespace_tokenized_command_size; 
} command_t;

command_t *create_command();
void destroy_command(command_t *command); 
void add_argument_to_command(command_t *command, char *argument);

#endif
