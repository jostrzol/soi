#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>

int main(int argc, char **argv)
{
    int status, tps, group;
    char groupChar;
    struct tms t;

    tps = sysconf(_SC_CLK_TCK);

    for (group=0; group<=1; group++) {
        groupChar = '0' + group;
        if (fork() == 0){
            if (fork() == 0) {
                execl("./group", "./group", &groupChar, NULL);
                exit(0);
            }
            wait(&status);
            if (!WIFEXITED(status)) {
                fprintf(stderr, "./group %d exited with status: %d\n", group, status);
                exit(1);
            }
            times(&t);
	    printf("group %d time: %f s\n", group, (float)t.tms_cutime/tps);
	    exit(0);
        }
    }
    wait(&status);
    wait(&status);
}
