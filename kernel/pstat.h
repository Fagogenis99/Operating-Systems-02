#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h" // for NPROC

struct pstat{
    int inuse[NPROC]; // whether this slot of the process table is in use (1 or 0)
    int pid[NPROC];   // the PID of each process
    int priority[NPROC]; // the priority level of each process
    int state[NPROC]; // the state of each process
    int ticks_used[NPROC]; // ticks used at current level
    int ticks_waiting[NPROC]; // ticks spent waiting in the current queue

    int ppid[NPROC]; // parent PID of each process
    uint64 size[NPROC]; // size of process memory
    char name[NPROC][16]; // name of each process
};

#endif // _PSTAT_H_