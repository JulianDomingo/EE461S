/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdlib.h>

#include "command.h"

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
    if (command->full_command) {
        free(command->full_command);
    }
    
    // Free the entries before freeing the entry holder.
    for (int token = 0; token < command->number_of_command_tokens; token++) {
        free(command->whitespace_tokenized_command[token]);
    }

    free(command->whitespace_tokenized_command);
}
