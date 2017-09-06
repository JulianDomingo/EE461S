// mem.c
// Original source: OSTEP
// Modified and annotated by Ramesh Yerraballi
// Memory example with dynamically alocated memory which gets its storage from the heap
//
// Build as: gcc -o mem mem.c -lpthread
// run with ./mem 100 A & ./mem 400 & ./mem 800  
//
// this demonstrates virtual CPUs as all
// three run at the same time, even if you
// only have one or two CPUs.  You can view
// your CPUs on Linux with "cat /proc/cpuinfo"
//
// this demonstrates virtual memory as each
// process may have the same address for
// "p", but each stores a different value there.
//
// Note that if you run this, you may happen
// to get different addresses for p.  This
// is likely if your OS randomizes addresses
// (this makes it harder for hackers to take
// advantage of bugs)
// You can turn off randomization
// with this command:
//
// sudo sysctl -w kernel.randomize_va_space=0
//
// You can turn it back on with this command
// 
// sudo sysctl -w kernel.randomize_va_space=2


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common.h"
int main(int argc, char *argv[])
{
    if (argc != 2) { 
	fprintf(stderr, "usage: mem <value>\n"); 
	exit(1); 
    } 
    int *p;                   // memory for pointer is on "stack"
    p = malloc(sizeof(int));  // malloc'd memory is on "heap"
    assert(p != NULL);
    printf("(pid:%d) addr of p:        %p\n", (int) getpid(), p);
    // the %p format option in printf allows clean printing of addresses in hex
     //     printf("(pid:%d) addr stored in p: %llx\n", (int) getpid(), (unsigned long long) p);
    *p = atoi(argv[1]); // assign value to addr stored in p
    while (1) {
	Spin(1);
	*p = *p + 1;
	printf("(pid:%d) value of p: %d\n", getpid(), *p);
    }

    return 0;
}

