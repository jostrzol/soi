#include <minix/callnr.h>
#include <lib.h>
#include <minix/type.h>
#include <stdlib.h>
#include "syscalls.h"

int setgroup(int group){
    message msg;
    msg.m_u.m_m1.m1i1 = group;
    return _syscall(MM, SETGROUP, &msg);
}

int getgroup(void){
    message msg;
    return _syscall(MM, GETGROUP, &msg);
}

int setpercenta(int percent_a){
    message msg;
    msg.m_u.m_m1.m1i1 = percent_a;
    return _syscall(MM, SETPERCENTA, &msg);
}
