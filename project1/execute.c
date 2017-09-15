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
 * Executes the command stored in the command_t variable.
 */
/*void execute_command(command_t *command, process_group_t *process_group_of_command, int *pipefd) {*/
    /*int file_descriptor;*/

    /*pid_t child_pid = fork();*/

    /*if (child_pid == 0) {*/
        /*// Child*/
        /*setpgid(0, process_group_of_command->process_group_id);        */

        /*// File redirections*/
        /*if (command->contains_redirect_stdin) {*/
            /*file_descriptor = open(command->redirect_stdin_filename, O_RDONLY);*/
            /*dup2(file_descriptor, STDIN_FILENO);*/
            /*close(file_descriptor);*/
        /*}*/
        /*if (command->contains_redirect_stdout) {*/
            /*file_descriptor = open(command->redirect_stdout_filename, O_RDONLY);*/
            /*dup2(file_descriptor, STDIN_FILENO);*/
            /*close(file_descriptor);*/
        /*}*/
        /*if (command->contains_redirect_stderr) {*/
            /*file_descriptor = open(command->redirect_stderr_filename, O_RDONLY);*/
            /*dup2(file_descriptor, STDIN_FILENO);*/
            /*close(file_descriptor);*/
        /*}*/

        /*execvp(command->whitespace_tokenized_command[0], command->whitespace_tokenized_command);*/
    /*}*/
    /*else {*/
        /*// Parent */
        /*setpgid(child_pid, process_group_of_command->process_group_id); */
    /*}*/
/*}*/

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

void handle_single_command(yash_shell_t *yash) {
    int status;

    process_group_t *active_process_group = yash->active_process_group;  

    pid_t child_pid = fork();

    if (child_pid == 0) {
        command_t *command = active_process_group->commands[0]; 

        handle_file_redirections(command);

        char **command_arguments = command->whitespace_tokenized_command;
        
        printf("Executing command '%s'\n", command_arguments[0]);
        execvp(command_arguments[0], command_arguments);
    }    
    else {
        // Parent
        if (active_process_group->is_foreground_job) {
            // Foreground job
            yash->fg_job = active_process_group; 
            waitpid(child_pid, &status, 0); 
        }
        else {
            // Background job
            move_job_to_bg(active_process_group, yash->bg_jobs_stack);
            waitpid(child_pid, &status, WNOHANG); 
        }
    }
}


void handle_double_commmand(yash_shell_t *yash) {

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

    // Create the pipe.
    /*int pipefd[2];  */

    /*pid_t child1_pid = fork();*/

    /*// Contains 2 commands*/
    /*if (child1_pid == 0) {*/
        /*// Group leader child (child 1)*/
        /*setsid();*/

        /*// Close unused read-end.*/
        /*close(pipefd[0]); */

        /*handle_file_redirections(active_process_group->commands[0]);*/

        /*dup2(pipefd[1], STDOUT_FILENO);*/

        /*// Execute the first command (read from left to right of the input string)*/
        /*// execute_command(active_process_group->commands[0], active_process_group); */
        /*char **command_arguments = active_process_group->commands[0]->whitespace_tokenized_command;*/
        /*execvp(command_arguments[0], command_arguments);*/
    /*}*/
    /*else {*/
        /*// Parent*/
        /*pid_t child2_pid = fork();*/

        /*if (child2_pid == 0) {*/
            /*// Child 2*/

            /*// Join with the process group of child1 (the group leader).*/
            /*setpgid(0, child1_pid);*/

            /*// Closed unused write-end*/
            /*close(pipefd[1]);*/

            /*handle_file_redirections(active_process_group->commands[1]);*/

            /*dup2(pipefd[0], STDIN_FILENO);*/

            /*// execute_command(active_process_group->commands[1], active_process_group);*/
            /*char **command_arguments = active_process_group->commands[1]->whitespace_tokenized_command;*/
            /*execvp(command_arguments[1], command_arguments);*/
        /*}*/
        /*else {*/
            /*// Parent*/

            /*// Close unused pipes.*/
            /*close(pipefd[0]);*/
            /*close(pipefd[1]);*/

            /*// Group together the commands in the process group by PID*/
            /*active_process_group->process_status = RUNNING;*/
            /*active_process_group->process_group_id = child1_pid;*/

            /*for (int command = 0; command < max_commands_limit; command++) {*/
                /*active_process_group->commands[command]->process_group_id = child1_pid;*/
            /*}*/

            /*int status;*/

            /*// The parent should only wait for foreground jobs.*/
            /*if (active_process_group->is_foreground_job) {*/
                /*yash->fg_job = active_process_group; */
                /*waitpid(active_process_group->process_group_id, &status, 0); */
            /*}*/
            /*else {*/
                /*// Background job*/
                /*move_job_to_bg(active_process_group, yash->bg_jobs_stack);*/
                /*waitpid(active_process_group->process_group_id, &status, WNOHANG); */
            /*}*/
        /*}*/
    /*}    */
}
