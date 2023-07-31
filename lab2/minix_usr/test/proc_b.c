#include "syscalls.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int i, j;

    printf("Initial group: %d\n", getgroup());
    setgroup(1);
    printf("Group after setgroup: %d\n", getgroup());

    while (1) {
	continue;
    }

    return 0;
}
