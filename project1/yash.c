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

#include "yash.h"
#include "bg_jobs.h"
#include "process_group.h"
#include "parse.h"

int status;
int pipefd[2];
char **parsed_input;
bool show_terminal_prompt;
pid_t process_group_id;
pid_t pid_child1;
pid_t pid_child2;

volatile atomic_t signal_from_child_process;

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/sigaction.html
 */
static void sighandler(int signum) {
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
    // initialize yash shell
    struct yash_t yash;
    yash.process_id = getpid();
    yash.bg_jobs_list = malloc(sizeof(head));
    yash.active_process_group = NULL;
    yash.fg_job = NULL;

    // initialize sigaction struct to handle received signals from child processes
    // http://pubs.opengroup.org/onlinepubs/009695399/functions/sigaction.html
    struct sigaction sa;
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);

    // Handles signal errors 
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction(SIGINT) error");
    }
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        perror("sigaction(SIGTSTP) error");
    } 
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction(SIGCHLD) error");
    } 
   
    // Handles pipe error
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    show_terminal_prompt = true;

    while (true) {
        if (show_terminal_prompt) {
            printf("%s", "# ");
        }
   
        // Handle signal interruptions first
        process_group_t fg_job = yash.fg_job; 

        switch (signal_from_child_process) {
            case SIGINT:
                if (fg_job) {
                    killpg(fg_job.process_group_id, SIGINT); 
                } 
                printf("\n");
                signal_from_child_process = 0;
                break;

            case SIGTSTP:
                if (fg_job) {
                    killpg(fg_job.process_group_id, SIGTSTP); 
                }
                printf("\n");
                signal_from_child_process = 0;
                break;
            
            case SIGCHLD:
                if (fg_job) {
                    // reap zombie process
                    destroy_process_group(&fg_job); 
                }
                printf()
                signal_from_child_process = 0;
                break;
           
            default:
                // No signal received
                break;
        }

        // read shell command 
        fgets(shell_input, MAX_CHARACTER_LIMIT, stdin);
        shell_input[strlen(shell_input) - 1] = '\0';
        
        // check if no command entered
        if (strlen(trim(shell_input)) == 0) { 
            continue;
        }
        else {
            parse_input(&shell_input, &yash);
            execute_input(&yash);
        }
    }
}
