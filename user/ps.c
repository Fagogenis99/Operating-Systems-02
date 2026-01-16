#include "kernel/types.h"
#include "kernel/pstat.h"
#include "kernel/user.h"

int main(int argc, char *argv[]){ //
    struct pstat stat; 

    if(getpinfo(&stat) < 0){
        printf("getpinfo failed\n");
        exit(1);
    }
    exit(0);
}