/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "parse.h"
#include "yash.h"
#include "process_group.h"

#define MAX_CHARACTER_LIMIT = 2000;

/*
 * Parses the input read from STDIN, 
 * dependent on the content of the command given.
 */
bool parse_input(char *shell_input, yash_t *yash) {
    char *trimmed_shell_input = trim(shell_input);     

    struct process_group_t *process_group = malloc(sizeof(process_group_t)); 
    initialize_process_group(process_group, shell_input);

    char **command;
    const char delimiter[2]= " ";
    char *token;

    // retrieve the first token with input string separately to startup tokenizer 
    token = strtok(shell_input, delimiter);

    while (token != NULL) {
        if (strcmp(token, "fg") == 0) {
            if (yash->fg_job) {
                    
            }
        }
        else if (strcmp(token, "bg") == 0) {

        }
        else if (strcmp(token, "jobs") == 0) {

        }
        else if (strcmp(token, "|") == 0) {
        
        }
        else if (strcmp(token, "<") == 0) {

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

        // retrieve next token
        token = strtok(NULL, delimiter);
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

/*
 * Retrieves the the active process group and executes its commmand(s).
 */
void execute_input(yash_t *yash) {
    process_group_t *active_process_group = yash->active_process_group;
    // TODO: Implement the logic to execute the commands stored in activate process group 
} 
