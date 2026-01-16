#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int main(int argc, char *argv[]){ //
    struct pstat stat; 

    if(getpinfo(&stat) < 0){
        printf("getpinfo failed\n");
        exit(1);
    }

    printf("PID\tPRIORITY\tSTATE\tTICKS_USED\tTICKS_WAITING\n");
    for (int i=0; i<NPROC; i++){ // printing process info
        if (stat.inuse[i]){
            printf("%d\t%d\t\t%d\t%d\t\t%d\n", stat.pid[i], stat.priority[i], stat.state[i], stat.ticks_used[i], stat.ticks_waiting[i]);
        }
    }
    exit(0);
}