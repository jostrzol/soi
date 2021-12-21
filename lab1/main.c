#include "/usr/include/minix/callnr.h"
#include "/usr/include/lib.h"
#include "/usr/include/minix/type.h"
#include <stdio.h>
#include <stdlib.h>

int getprocnr(int proc_id)
{
        message *msg = (message *)malloc(sizeof(message));
        msg->m_u.m_m1.m1i1 = proc_id;
        return _syscall(MM, GETPROCNR, msg);
}

int main(int argc, char **argv)
{
        int proc_id, proc_nr, proc_id_end;

        if (argc != 2)
        {
                fprintf(stderr, "usage: %s PID\n", argv[0]);
                return -1;
        }

        proc_id = atoi(argv[1]);
        proc_id_end = proc_id + 10;

        for (proc_id; proc_id <= proc_id_end; proc_id++)
        {
                proc_nr = getprocnr(proc_id);
                if (proc_nr < 0)
                        printf("%d: ERROR: %d\n", proc_id, errno);
                else
                        printf("%d: %d\n", proc_id, proc_nr);
        }

        return 0;
}
