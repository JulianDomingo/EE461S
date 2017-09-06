#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

const int MAX_CHARACTER_LIMIT = 2000;

static void sig_handler(int signal_number) {
    exit(0);
} 

static void parse_input(char* shell_input) {
     
}

int main() {
    char shell_input[MAX_CHARACTER_LIMIT + 1];
    
    while (true) {

        printf("%s", "# ");
        scanf("%s", shell_input);
       
        parse_input(shell_input);

        pid_t process_id = fork();

    }
}
