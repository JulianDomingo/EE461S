// cpu.c
// Original source: OSTEP
// Modified and annotated by Ramesh Yerraballi
// CPU virtualization example showing how multiple processes seem to run apparently at the same time
//
// Build as: gcc -o cpu cpu.c -lpthread
// run with ./cpu A & ./cpu B & ./cpu C 
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
	fprintf(stderr, "usage: cpu <string>\n");
	exit(1);
    }
    char *str = argv[1];

    while (1) {
	printf("%s\n", str);
	Spin(1);
    }
    return 0;
}

