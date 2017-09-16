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

static const int max_commands_limit = 2;

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
 * Handles pipelines containing a single command differently from pipeliens with two commands.
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
        if (active_process_group->is_foreground_job) {
            // Foreground job
            yash->fg_job = active_process_group; 

            // Wait for the foreground job to complete.
            while (waitpid(child1_pid, &status, 0) != child1_pid); 
        }
        else {
            // Background job
            move_job_to_bg(active_process_group, yash->bg_jobs_stack);

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

            int count = 0;
            while (count < 2) {
                pid_t pid = waitpid(-1, &status, WUNTRACED | WCONTINUED);

                if (WIFEXITED(status)) {
                    count++; 
                }
                else if (WIFSIGNALED(status)) {
                    count++;
                }
                else if (WIFSTOPPED(status)) {
                    sleep(4);
                    kill(pid, SIGCONT);
                }
            }
            /*if (active_process_group->is_foreground_job) {*/
                /*// Foreground job*/
                /*yash->fg_job = active_process_group; */

                /*// Wait for the foreground job to complete.*/
                /*while (waitpid(child1_pid, &status, 0) != child1_pid); */
            /*}*/
            /*else {*/
                /*// Background job*/
                /*move_job_to_bg(active_process_group, yash->bg_jobs_stack);*/

                /*// Don't wait for the background job to complete.*/
                /*waitpid(child1_pid, &status, WNOHANG); */
            /*}*/
        }
        else {
            // Child 2

            // Avoids race condition
            sleep(1); 
            setpgid(0, child1_pid);   

            close(pipefd[1]);  
            dup2(pipefd[0], STDIN_FILENO);

            char **second_command_arguments = active_process_group->commands[1]->whitespace_tokenized_command;
            printf("Executing command 2: '%s %s'\n", second_command_arguments[0], second_command_arguments[1]);
            execvp(second_command_arguments[0], second_command_arguments);
        }
    }
}

/*
 * Retrieves the the active process group and executes its commmand(s).
 */
void execute_input(yash_shell_t *yash) {
    process_group_t *active_process_group = yash->active_process_group;

    if (active_process_group->commands_size == 1) {
        handle_single_command(yash);
    }
    else {
        handle_double_commmand(yash);
    }
}
