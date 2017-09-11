/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parse.h"
#include "process_group.h"
#include "bg_jobs.h"
#include "yash.h"

int status;
int pipefd[2];
char **parsed_input;
bool show_terminal_prompt;
volatile sig_atomic_t signal_from_child_process;

/*
 * Handler for needed signals to implement.  
 */
static void sig_handler(int signum) {
    switch (signum) {
        case SIGINT:
            signal_from_child_process = SIGINT; 
            break; 
        case SIGTSTP:
            signal_from_child_process = SIGTSTP;
            break;
        case SIGCHLD:
            signal_from_child_process = SIGCHLD;
            break;
    }
}

int main() {    
    char shell_input[MAX_CHARACTER_LIMIT];

    // initialize yash shell
    struct yash_t yash;
    yash.process_id = getpid();
    yash.active_process_group = NULL;
    yash.fg_job = NULL;
    yash.bg_jobs_stack = malloc(sizeof(head));

    // initialize sigaction struct
    struct sigaction sa; 
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);

    // Handles signal errors 
    if (sigaction(SIGINT, &sa, NULL) == -1) { 
        printf("signal(SIGINT) error");
    } 
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        printf("signal(SIGTSTP) error");
    }
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        printf("signal(SIGCHLD) error");
    } 

    // Handles pipe error
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    show_terminal_prompt = true;

    while (true) {
        // Handle signal interruptions first
        process_group_t *foreground_job = yash.fg_job; 

        switch (signal_from_child_process) {
            case SIGINT:
                if (foreground_job) {
                    // Send SIGINT to the foreground job 
                    killpg(foreground_job->process_group_id, SIGINT);
                }
                fputc('\n', stdout);
                signal_from_child_process = 0;
                break;

            case SIGTSTP:
                if (foreground_job) {
                    // Send SIGTSTP to the foreground job 
                    killpg(foreground_job->process_group_id, SIGTSTP); 
                    // TODO: Move the foreground job to background
                }
                fputc('\n', stdout);
                signal_from_child_process = 0;
                break;
            
            case SIGCHLD:
                if (foreground_job) {
                    // TODO: recognize child process has terminated by removing the foreground job
                    // from the yash session 
                }
                signal_from_child_process = 0;
                break;
            
            default:
                // No signal, do nothing 
                break;
        }

        if (show_terminal_prompt) {
            printf("%s", "# ");
        }
        
        // read shell command 
        fgets(shell_input, MAX_CHARACTER_LIMIT, stdin);
        shell_input[strlen(shell_input) - 1] = '\0';

        // check if no command entered
        if (strlen(trim(shell_input)) == 0) { 
            continue;
        }
        else {
            // retrieve command(s) and execute them
            /*parse_input(&shell_input, &yash);*/
            /*execute_input(&yash);*/
        }
    }
}
