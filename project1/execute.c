/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "command.h"
#include "execute.h"
#include "yash_shell.h"
#include "process_group.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

static const int max_commands_limit = 2;
volatile sig_atomic_t signal_interrupt;

/*
 * Signal handler called when interrupting system calls: 
 *  1. Handling Ctrl-C to a foreground job for termination
 *  2. Handling Ctrl-Z to a foreground job for suspension + transfer to background
 *  3. Handling continuation of a suspended job. 
 */
static void sig_handler(int signum) {
    switch (signum) {
        case SIGINT:
            signal_interrupt = SIGINT; 
            break; 
        case SIGTSTP:
            signal_interrupt = SIGTSTP;
            break;
        case SIGCONT:
            signal_interrupt = SIGCONT;
            break;
        default: 
            break;
    }
}

/*
 * Handles file redirections for the given command should they exist.
 *
 * 1. Retrieve the file descriptor index for the given file.
 * 2. Depending on the redirection symbol, replace the STD<X> with the file descriptor 
 *    index of the opened file.
 * 3. Close the file.
 */
void handle_file_redirections(command_t *command) {
    int file_descriptor;

    if (command->contains_redirect_stdin) {
        file_descriptor = open(command->redirect_stdin_filename, O_RDONLY);
        dup2(file_descriptor, STDIN_FILENO);
        close(file_descriptor);
    }
    if (command->contains_redirect_stdout) {
        file_descriptor = open(command->redirect_stdout_filename, O_RDWR | O_CREAT, S_IRWXU); 
        dup2(file_descriptor, STDOUT_FILENO);
        close(file_descriptor);
    }
    if (command->contains_redirect_stderr) {
        file_descriptor = open(command->redirect_stderr_filename, O_RDWR | O_CREAT, S_IRWXU); 
        dup2(file_descriptor, STDERR_FILENO);
        close(file_descriptor);
    }
}

/*
 * Handles input containing a single command differently from input with two commands.
 */
void handle_single_command(yash_shell_t *yash) {
    int status;

    process_group_t *active_process_group = yash->active_process_group;  

    pid_t child1_pid = fork();

    if (child1_pid == 0) {
        command_t *command = active_process_group->commands[0]; 
        handle_file_redirections(command);

        char **command_arguments = command->whitespace_tokenized_command;

        execvp(command_arguments[0], command_arguments);
    }    
    else {
        // Parent
        active_process_group->process_status = RUNNING;

        if (active_process_group->is_foreground_job) {
            // Foreground job
            yash->fg_job = active_process_group; 

            // Wait for the foreground job to complete.
            while (waitpid(child1_pid, &status, 0) != child1_pid) {
                // TODO: use same code inside handle_double_commmand() for this while loop in common function 
            }
            
            // Delay of a tenth of a second ensuring any output to STDOUT is seen before the next '#' prompt.
            usleep(100000);
        }
        else {
            printf("Background job detected!\n");
            // Background job
            move_job_to_bg(active_process_group, yash->bg_jobs_linked_list);

            // Don't wait for the background job to complete.
            waitpid(child1_pid, &status, WNOHANG); 
        }
    }
}


/*
 * Handles shell inputs with two commands (contains a single '|' symbol). 
 */
void handle_double_commmand(yash_shell_t *yash) {
    // pipefd[0] == read-end, pipefd[1] == write-end
    int pipefd[2];
    int status;

    if (pipe(pipefd) == -1) {
        perror("pipe error lol"); 
        exit(EXIT_SUCCESS);
    }

    process_group_t *active_process_group = yash->active_process_group;

    if (active_process_group->commands_size != 2) {
        perror("Parser did not successfully gather commands.");
        exit(EXIT_FAILURE);
    }

    pid_t child1_pid = fork();

    if (child1_pid == 0) {
        // Child 1 (group leader)
        pid_t session_id = setsid();

        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);

        char **first_command_arguments = active_process_group->commands[0]->whitespace_tokenized_command;
        execvp(first_command_arguments[0], first_command_arguments);
    }
    else {
        // Parent
        pid_t child2_pid = fork();

        if (child2_pid > 0) {
            // Parent
            close(pipefd[0]);
            close(pipefd[1]);

            active_process_group->process_status = RUNNING;

            if (active_process_group->is_foreground_job) {
                // Foreground job
                yash->fg_job = active_process_group; 

                bool signal_received = false;

                // Wait for the foreground job to complete.
                while (waitpid(child1_pid, &status, 0) != child1_pid) {
                    // 'errno' is set to 'EINTR' when 'WNOHANG' isn't passed to waitpid() and an unblocked signal or 'SIGCHLD' was caught.
                    if (errno == EINTR) {
                        switch (signal_interrupt) {
                            case SIGINT:
                                printf("Received interrupting Ctrl-C!\n");
                                signal_received = true;
                                killpg(active_process_group->process_group_id, SIGINT);                                
                    
                                remove_linked_list_node(active_process_group, yash->bg_jobs_linked_list);
                                signal_interrupt = 0;

                                // Terminate this child (which will send SIGCHLD to the parent process)
                                exit(EXIT_SUCCESS);

                            case SIGTSTP:
                                printf("Received interrupting Ctrl-Z!\n");
                                signal_received = true;
                                
                                killpg(active_process_group->process_group_id, SIGTSTP);                                
                                move_job_to_bg(active_process_group, yash->bg_jobs_linked_list);
                                
                                printf("[%d] + %s    %s\n", 
                                        yash->bg_jobs_linked_list->size,
                                        "Stopped", 
                                        active_process_group->full_command);
                                signal_interrupt = 0;
                                break;

                            case SIGCONT:
                                printf("Received continuation signal!\n");
                                // Don't break from waitpid() block, as yash needs to wait for foreground jobs to terminate
                                killpg(active_process_group->process_group_id, SIGCONT);                                
                                signal_interrupt = 0;
                                break;
                            
                            default:
                                // Do nothing if no signal was received.
                                break;
                        }                          
                    } 

                    if (signal_received) {
                        // Stop blocking if termination / suspension signal is received
                        break;
                    }
                }

                // Delay of a tenth of a second ensuring any output to STDOUT from the system call is seen before the next '#' prompt.
                usleep(100000);
            }
            else {
                // Background job
                move_job_to_bg(active_process_group, yash->bg_jobs_linked_list);

                // Don't wait for the background job to complete.
                waitpid(child1_pid, &status, WNOHANG); 
            }
        }
        else {
            // Child 2
            setpgid(0, child1_pid);   

            close(pipefd[1]);  
            dup2(pipefd[0], STDIN_FILENO);

            char **second_command_arguments = active_process_group->commands[1]->whitespace_tokenized_command;
            execvp(second_command_arguments[0], second_command_arguments);
        }
    }
}

/*
 * Retrieves the the active process group and executes its commmand(s).
 */
void execute_input(yash_shell_t *yash) {
    process_group_t *active_process_group = yash->active_process_group;

    // Initialize signals
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        printf("signal(SIGINT) error");
    }
    if (signal(SIGTSTP, sig_handler) == SIG_ERR) {
        printf("signal(SIGTSTP) error");
    }
    if (signal(SIGCONT, sig_handler) == SIG_ERR) {
        printf("signal(SIGCONT) error");
    }

    if (active_process_group->commands_size == 1) {
        handle_single_command(yash);
    }
    else {
        handle_double_commmand(yash);
    }
}
