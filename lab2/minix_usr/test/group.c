#include "syscalls.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int i, j, group;

    if (argc != 2){
        fprintf(stderr, "usage: %s GROUP\n", argv[0]);
        return 1;
    }

    group = atoi(argv[1]);
    if (group < 0 || group > 1){
    	fprintf(stderr, "group must be 0 or 1\n");
    	return 2;
    }

    printf("Initial group: %d\n", getgroup());
    setgroup(group);
    printf("Group after setgroup: %d\n", getgroup());

    printf("group: %d, progress: %d%%\n", group, 0);
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 30000000; j++){ continue; }
        printf("group: %d, progress: %d%%\n", group, (i+1)*10);
    }

    return 0;
}
