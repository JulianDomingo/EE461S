#ifndef _EXECUTE_H_
#define _EXECUTE_H_

/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "command.h"
#include "process_group.h"
#include "yash_shell.h"

void execute_input(yash_shell_t *yash); 
void execute_command(command_t *command, process_group_t *process_group_of_command, int *pipefd); 

#endif
