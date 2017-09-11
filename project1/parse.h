/* 
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include <stdbool.h>

#include "yash.h"

extern const int MAX_CHARACTER_LIMIT;

static bool parse_input(char *shell_input, yash_t *yash);
static void execute_input(yash_t *yash); 
static char *trim(char *untrimmed);
