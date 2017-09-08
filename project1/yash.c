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
#include "jobs.h"

const int MAX_CHARACTER_LIMIT = 2000;

int status;
int pipefd[2];
char **parsed_input;
pid_t process_group_id;
pid_t pid_child1;
pid_t pid_child2;

/*
 * Sends a signal from the parent process to the processes
 * in the current process group to terminate.
 */
static void sig_int() {
    kill(-process_group_id, SIGINT);
} 

/*
 * Sends a signal from the parent process to the processes
 * in the current process group to suspend.
 */
static void sig_tstp() {
    kill(-process_group_id, SIGTSTP);
}

/*
 * Sends a signal from the child process to the parent process (yash) indicating the child process has either terminated or suspended. 
 */
static void sig_chld() {
    kill(-pid_shell, SIGCHLD);
}

/*
 * Parses the input read from STDIN to be readied for exec'ing.
 */
static void parse_input(char *shell_input) {
    // TODO: implement
}

/*
 * Sets the process group ID as its own process ID, reroutes 
 * command 1 output from STDOUT to write-end of pipe, and execs command. 
 */
void handle_child1() {
    setsid();
    close(pipefd[0]);     
    dup2(pipefd[1], STDOUT_FILENO); 

    execvp(parsed_input[0], parsed_input);
}

void handle_child2() {

}

int main() {    
    char shell_input[MAX_CHARACTER_LIMIT + 1];
    
    // initialize yash shell
    struct yash_t yash;
    yash.process_id = getpid();
    yash.bg_jobs_list = malloc(sizeof(bg_jobs));
    yash.current_process_group = NULL;
   
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // continue reading shell commands
    while (true) {
        printf("%s", "# ");
    
        fgets(shell_input, MAX_CHARACTER_LIMIT, stdin);

        parse_input(&shell_input);

        pid_child1 = fork();

        if (pid_child1 > 0) {
            // Parent process 
            pid_child2 = fork();

            if (pid_child2 > 0) {
                // Parent process  
                close(pipefd[0]); 
                close(pipefd[1]);
               
                int children_exited_or_killed = 0; 

                while (children_exited_or_killed < 2) {
                    pid_t pid = waitpid(-1, &status, WUNTRACED | WCONTINUED);  

                    if (pid == -1) {
                        perror("waitpid");
                        exit(EXIT_FAILURE);
                    }

                    if (WIFEXITED(status)) {

                    }
                    else if (WIFEXITED(status)) {

                    }
                    else if (WIFSTOPPED(status)) {

                    }
                    else if (WIFCONTINUED(status)) {

                    }
                }

                exit(EXIT_SUCCESS);
            }
            else {
                handle_child2();
            }
        }
        else {
            handle_child1(); 
        }
    }
}

