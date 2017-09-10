#include <stdbool.h>
#include <ctype.h>

#include "parse.h"
#include "yash.h"

const int MAX_CHARACTER_LIMIT = 2000;

/*
 * Parses the input read from STDIN, and takes appropriate action
 * dependent on the content of the command given.
 */
static bool parse_input(char *shell_input, yash_t *yash) {
    // TODO: Parse input
    // TODO: Implement logic for each case (pipe, file redirect, job control)
    // TODO: Update corresponding struct member fields for relevant objects
        
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
