/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "command.h"
#include "process_group.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * Creates and returns a new heap-allocated command_t variable. 
 */
command_t *create_command() {
    command_t *new_command = malloc(sizeof(command_t));
    new_command->whitespace_tokenized_command = malloc(sizeof(char *));
    return new_command;
}

/*
 * Frees all heap-allocated values of the passed command_t variable.
 */
void destroy_command(command_t *command) {
    if (command->redirect_stdin_filename) {
        free(command->redirect_stdin_filename);
    }    
    if (command->redirect_stdout_filename) { 
        free(command->redirect_stdout_filename);
    }
    if (command->redirect_stderr_filename) {
        free(command->redirect_stderr_filename); 
    }
    
    // Free the entries before freeing the entry holder.
    for (int token = 0; token < command->whitespace_tokenized_command_size; token++) {
        free(command->whitespace_tokenized_command[token]);
    }

    free(command->whitespace_tokenized_command);
}

/*
 * Adds an additional argument to the passed command_t variable.
 */
void add_argument_to_command(command_t *command, char *argument) {
    command->whitespace_tokenized_command[command->whitespace_tokenized_command_size] = argument;
    command->whitespace_tokenized_command_size++;
}
