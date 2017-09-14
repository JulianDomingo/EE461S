/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "parse.h"
#include "process_group.h"
#include "yash_shell.h"

#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_CHARACTER_LIMIT = 2000;

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

    char **command;
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
                    printf("%s\n", most_recent_process_group_in_bg->command_entered);
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
                    printf("[%d] + Running    %s\n", yash->bg_jobs_stack->size, most_recent_process_group_in_bg->command_entered);

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
                                runner->process_group->command_entered);

                        job_number++;
                    }
                }    
            }

            // Do not fork after this function and immediately continue to the next input 
            return false;
        }
        else if (strcmp(token, "|") == 0) {

        }
        else if (strcmp(token, "<") == 0) {
            char *redirect_stdin_filename = strtok(NULL, delimiter);
    
            FILE *file;    
            
            // Check that the file exists.
            if (fopen(redirect_stdin_filename, "r")) {
                 
            }
            else {
                printf("yash: %s: No such file or directory\n", redirect_stdin_filename); 
            }
        }
        else if (strcmp(token, ">") == 0) {

        }
        else if (strcmp(token, "2>") == 0) {

        }
        else if (strcmp(token, "&") == 0) {
            // insert the process group into bg job stack
            
        }
        else {
            // Either command name or flag 

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

