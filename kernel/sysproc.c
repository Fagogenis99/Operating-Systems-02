#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "pstat.h" // added for pstat struct

uint64
sys_getpinfo(void)
{
  uint64 pstat_addr;
  struct pstat stat;
  struct proc *p;
  extern struct proc proc[NPROC];
  int i = 0;

  argaddr(0, &pstat_addr); // get the user pointer argument

  for (p=proc; p<&proc[NPROC]; p++) { // iterate through process table
    acquire(&p->lock);
    if (p->state != UNUSED) {
      stat.inuse[i] = 1;
      stat.pid[i] = p->pid;
      stat.priority[i] = p->priority; // added for priority
      stat.state[i] = p->state;
      stat.ticks_used[i] = p->ticks_used; // added for ticks_used
      stat.ticks_waiting[i] = p->ticks_waiting; // added for ticks_waiting

      stat.size[i] = p->sz;
      if (p->parent) {
        stat.ppid[i] = p->parent->pid;
      } else {
        stat.ppid[i] = 0; // no parent
      }
      safestrcpy(stat.name[i], p->name, sizeof(p->name));
    } else {
      stat.inuse[i] = 0;
    }
    release(&p->lock);
    i++;
  }
  if (copyout(myproc()->pagetable, pstat_addr, (char *)&stat, sizeof(stat)) < 0) {
    return -1;
  }
  return 0;

}

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
