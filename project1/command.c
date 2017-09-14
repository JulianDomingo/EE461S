/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdlib.h>

#include "command.h"

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

/*
 * Executes the command stored in the command_t variable.
 */
void execute_command(command_t *command, process_group *process_group_of_command) {
    int file_descriptor;

    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child
        
        // File redirections
        if (command->contains_redirect_stdin) {
            file_descriptor = open(command->redirect_stdin_filename, "r");
            dup2(file_descriptor, STDIN_FILENO);
            close(file_descriptor);
        }
        if (command->contains_redirect_stdout) {
            file_descriptor = open(command->redirect_stdout_filename, "r");
            dup2(file_descriptor, STDIN_FILENO);
            close(file_descriptor);
        }
        if (command->contains_redirect_stderr) {
            file_descriptor = open(command->redirect_stderr_filename, "r");
            dup2(file_descriptor, STDIN_FILENO);
            close(file_descriptor);
        }

        // Piping
        // TODO:
    }
    else {
        // Parent 
        // TODO:
    }
}
