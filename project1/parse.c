/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "parse.h"
#include "process_group.h"
#include "yash_shell.h"
#include "command.h"

#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Parses the input read from STDIN, 
 * dependent on the content of the command given.
 * Returns:
 *      true if yash should fork after parsing of the shell input. 
 *      false if yash should NOT fork after parsing of the shell input (i.e. job control commands). 
 *
 * NOTE: This lab requires handling of ONE kind of invalid input, which is the only invalid
 * input checked for: non-existent redirect files.
 */
bool parse_input(char *shell_input, yash_shell_t *yash) {
    char *trimmed_shell_input = trim(shell_input);     

    process_group_t *process_group = malloc(sizeof(process_group_t)); 
    initialize_process_group(process_group, shell_input);

    command_t *new_command = create_command();

    const char delimiter[2]= " ";
    char *token;

    // retrieve the first token with input string separately to startup tokenizer 
    token = strtok(shell_input, delimiter);

    while (token != NULL) {
        if (strcmp(token, "fg") == 0 ||
            strcmp(token, "bg") == 0 ||
            strcmp(token, "jobs") == 0)
        {
            // Job control commands are dealt with using our own implementation. Therefore, no fork is necessary after this method. 
            destroy_command(new_command);
            destroy_process_group(process_group); 
            free(process_group);
           
            // status for waitpid
            int status;

            if (strcmp(token, "fg") == 0) {
                process_group_t *most_recent_process_group_in_bg = yash->bg_jobs_stack->pointer_to_head->next->process_group;
               
                if (!most_recent_process_group_in_bg) {
                    printf("yash: fg: current: no such job");
                }
                else {
                    killpg(most_recent_process_group_in_bg->process_group_id, SIGCONT);
                    printf("%s\n", most_recent_process_group_in_bg->full_command);
                    move_job_to_fg(yash);
                    most_recent_process_group_in_bg->process_status = RUNNING;

                    // Wait for completion of the process, indicated by the "0" argument.
                    waitpid(most_recent_process_group_in_bg->process_group_id, &status, 0);
                }
            }
            else if (strcmp(token, "bg") == 0) {
                process_group_t *most_recent_process_group_in_bg = yash->bg_jobs_stack->pointer_to_head->next->process_group;

                if (!most_recent_process_group_in_bg) {
                    printf("yash: bg: current: no such job");
                }
                else if (most_recent_process_group_in_bg->process_status == RUNNING) {
                    printf("yash: bg: job already running in background"); 
                }
                else {
                    killpg(most_recent_process_group_in_bg->process_group_id, SIGCONT);
                    most_recent_process_group_in_bg->process_status = RUNNING;
                    printf("[%d] + Running    %s\n", yash->bg_jobs_stack->size, most_recent_process_group_in_bg->full_command);

                    // Do NOT wait for completion (as if '&'), indicated by the "WNOHANG" argument. 
                    waitpid(most_recent_process_group_in_bg->process_group_id, &status, WNOHANG);
                }    
            }
            else {
                // "jobs" command. As per bash, this command outputs nothing if no jobs exist. 
                if (yash->bg_jobs_stack->size > 0) {
                    bg_jobs_stack_node_t *runner = yash->bg_jobs_stack->pointer_to_tail;

                    int job_number = 1; 
      
                    while (runner && !runner->is_head_or_tail) {
                        printf("[%d] %s %s    %s\n", 
                                job_number, 
                                (runner->previous) ? "+" : "-",  
                                (runner->process_group->process_status == RUNNING) ? "Running" : "Stopped",
                                runner->process_group->full_command);

                        job_number++;
                    }
                }    
            }

            // Do not fork after this function and immediately continue to the next input 
            return false;
        }
        else if (strcmp(token, "|") == 0) {
            // Guaranteed the command is finished, so add (excluding the pipe) to the current process group.            
            process_group->commands[process_group->commands_size] = new_command; 
            process_group->commands_size++;

            // Point to a new heap-allocated command.
            new_command = create_command();

            // Fetch the next token
            token = strtok(NULL, delimiter);
        }
        else if (strcmp(token, "<") == 0) {
            // Retrieve file name.
            token = strtok(NULL, delimiter);
    
            FILE *file_pointer = fopen(token, "r"); 
            
            // Check that the file exists.
            if (file_pointer) {
                new_command->redirect_stdin_filename = strdup(token);
                new_command->contains_redirect_stdin = true;
                fclose(file_pointer);
            }
            else {
                fclose(file_pointer);
                printf("yash: %s: No such file or directory\n", token); 
                
                destroy_command(new_command);
                destroy_process_group(process_group); 
                free(process_group);
                
                return false;
            }
        }
        else if (strcmp(token, ">") == 0) {
            // Retrieve file name.
            token = strtok(NULL, delimiter);
            new_command->redirect_stdout_filename = strdup(token);
            new_command->contains_redirect_stdout = true;
        }
        else if (strcmp(token, "2>") == 0) {
            // Retrieve file name.
            token = strtok(NULL, delimiter);
            new_command->redirect_stderr_filename = strdup(token);
            new_command->contains_redirect_stderr = true;
        }
        else if (strcmp(token, "&") == 0) {
            process_group->is_foreground_job = false; 
            process_group->commands[process_group->commands_size] = new_command; 
            process_group->commands_size++;

            // Can pre-emptively return since the '&' token will always be the last in a given input.
            return true;
        }
        else {
            // Token is either a command name or command argument; add normally to the command_t variable. 
            add_argument_to_command(process_group->commands[process_group->commands_size], token); 
            token = strtok(NULL, delimiter);
        }
    }
} 

/*
 * Trims all leading and trailing whitespace from the string.
 */
char *trim(char *string) {
    char *end;
   
    // remove leading whitespace
    while (isspace((unsigned char) *string)) {
        string++;
    }

    // the string is all spaces.
    if (*string == 0) { return string; }

    end = string + strlen(string) - 1;
   
    // remove trailing whitespace
    while (end > string && isspace((unsigned char) *end)) {
        end--;
    }

    // define new string terminator
    *(end + 1) = '\0';

    return string;
}

