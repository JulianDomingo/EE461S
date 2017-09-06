// threads.v0.c
// Original source: OSTEP
// Modified and annotated by Ramesh Yerraballi
// Concurrency example showing how multiple processes threads can manipulate a shared global variable
// This is bad programming as the two threads can cause a race condition 
//
// Copy it to threads.c and
// Build as: gcc -o threads threads.c -lpthread
// run as:   ./threads 200000
// change the input and observe

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

volatile int counter = 0; 
int loops;

void *worker(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	counter++;
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) { 
	fprintf(stderr, "usage: threads <loops>\n"); 
	exit(1); 
    } 
    loops = atoi(argv[1]);
    pthread_t p1, p2;
    printf("Initial value : %d\n", counter);
    Pthread_create(&p1, NULL, worker, NULL); 
    Pthread_create(&p2, NULL, worker, NULL);
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    printf("Final value   : %d\n", counter);
    return 0;
}

