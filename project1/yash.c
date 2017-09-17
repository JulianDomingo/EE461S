/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "execute.h"
#include "bg_jobs.h"
#include "parse.h"
#include "process_group.h"
#include "yash.h"
#include "yash_shell.h"

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int status;
int pipefd[2];
char **parsed_input;
bool show_terminal_prompt;
volatile sig_atomic_t signal_interrupt;

/*
 * Signal handler called from user input (Ctrl-C, Ctrl-Z) / terminated child processes. 
 */
static void sig_handler(int signum) {
    switch (signum) {
        case SIGINT:
            signal_interrupt = SIGINT; 
            break; 
        case SIGTSTP:
            signal_interrupt = SIGTSTP;
            break;
        case SIGCHLD:
            signal_interrupt = SIGCHLD;
            break;
        default:
            break;
    }
}

int main() {    
    char shell_input[MAX_CHARACTER_LIMIT];
    bool is_signal_input;

    // Initialize yash shell
    struct yash_shell_t yash;
    yash.process_id = getpid();
    yash.active_process_group = NULL;
    yash.fg_job = NULL;
    yash.bg_jobs_linked_list = create_bg_jobs_linked_list(); 

    // Initialize signals
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        printf("signal(SIGINT) error");
    }
    if (signal(SIGTSTP, sig_handler) == SIG_ERR) {
        printf("signal(SIGTSTP) error");
    }
    if (signal(SIGCHLD, sig_handler) == SIG_ERR) {
        printf("signal(SIGCHLD) error");
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
        process_group_t *foreground_job = yash.fg_job; 

        is_signal_input = false;

        switch (signal_interrupt) {
            case SIGINT:
                if (foreground_job) {
                    // Send SIGINT to the foreground job 
                    killpg(foreground_job->process_group_id, SIGINT);
                }
                signal_interrupt = 0;
                is_signal_input = true;
                break;

            case SIGTSTP:
                // As per bash, yash should do nothing if Ctrl-Z is sent to the shell when no job is present.
                signal_interrupt = 0;
                is_signal_input = true;
                break;
            
            case SIGCHLD:
                if (foreground_job) {
                    destroy_process_group(foreground_job);
                    yash.fg_job = NULL;
                }
                signal_interrupt = 0;
                // Don't set "is_signal_input" to true, because this signal is sent naturally
                break;
            
            default:
                // No signal, do nothing 
                break;
        }
        
        // read shell command 
        char *eof_checker = fgets(shell_input, MAX_CHARACTER_LIMIT, stdin);

        if (!eof_checker) {
            // Ctrl + D to fgets() returns NULL, so check for it to know when to exit yash.
            exit(EXIT_SUCCESS);
        }

        shell_input[strlen(shell_input) - 1] = '\0';

        if (!is_signal_input) {
            // check if no command entered
            if (strlen(trim(shell_input)) == 0) { 
                continue;
            }
            else {
                bool invoke_execute_input = parse_input(shell_input, &yash);

                if (invoke_execute_input) {
                    execute_input(&yash);
                } 
            }
        } 
    }
}
