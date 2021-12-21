#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	int percenta, result;
	if (argc != 2) {
		fprintf(stderr, "usage: %s PERCENTA\n", argv[0]);
		return 1;
	}
	percenta = atoi(argv[1]);
	if (percenta < 1 || percenta > 99){
		fprintf(stderr, "percenta must be between 1 and 99\n");
		return 2;
	}
	result = setpercenta(percenta);
	printf("TIMES_A = %d, TIMES_B = %d\n", result/100, result%100);
	return 0;
}
