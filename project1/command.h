#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdbool.h>

typedef struct command_t {
    // File redirection
    bool contains_redirect_stdin;
    bool contains_redirect_stout;
    bool contains_redirect_stderr;
    char *redirect_stdin_filename;
    char *redirect_stdout_filename;
    char *redirect_stderr_filename;

    // Command information 
    char *full_command;
    char *whitespace_tokenized_command; 
    int number_of_command_tokens;
} command_t;

void destroy_command(command_t *command); 

#endif
