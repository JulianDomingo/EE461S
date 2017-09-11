/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "parse.h"
#include "yash.h"

const int MAX_CHARACTER_LIMIT = 2000;

/*
 * Parses the input read from STDIN, 
 * dependent on the content of the command given.
 */
static bool parse_input(char **shell_input, yash_t *yash) {
    char *trimmed_shell_input = trim(*shell_input);     

    int length = strlen(*shell_input);

    char **command;
    const char[2] delimiter = " ";
    char *token;

    token = strtok(*shell_input, delimiter);

    while (token != NULL) {
        switch (token) {
            case "|":
                // Reached end of a command
                                 
                break;
            
            case "<":
                // STDIN
                break;

            case ">":
                // STDOUT
                break;

            case "2>":
                // STDERR
                break;

            default:
                // Either command name or flag 
    
                break;
        }


        // retrieve next token
        token = strtok(NULL, delimiter);
    }
} 

/*
 * Trims all leading and trailing whitespace from the "untrimmed" string.
 */
static char *trim(char *untrimmed) {
    char *end;
   
    // remove leading whitespace
    while (isspace((unsigned char) *untrimmed)) {
        untrimmed++;
    }

    // the string is all spaces.
    if (*untrimmed == 0) { return untrimmed; }

    end = str + strlen(str) - 1;
   
    // remove trailing whitespace
    while (end > str && isspace((unsigned char) *end)) {
        end--;
    }

    // define new string terminator
    *(end + 1) = '\0';

    return untrimmed;
}

/*
 * Retrieves the the active process group and executes its commmand(s).
 */
static void execute_input(yash_t *yash) {

} 
