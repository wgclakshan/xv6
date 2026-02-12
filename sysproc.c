#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


// Needed for the third system call
int my_secret_kernel_var = 0; 

// Needed for the fourth system call (to get the address of sys_fork)
extern int sys_fork(void);

int
sys_getkernelstartaddr(void)
{
  // Returns the starting virtual address of the kernel (KERNBASE)
  return KERNBASE;
}

int
sys_getkernelendaddr(void)
{
  // Returns the ending virtual address (PHYSTOP + KERNBASE)
  return KERNBASE + PHYSTOP;
}

int
sys_getkernelvariaddr(void)
{
  // Returns the address of a variable in kernel space
  return (int)&my_secret_kernel_var;
}

int
sys_getsystemcalladdr(void)
{
  // Returns the address of the sys_fork function itself
  return (int)sys_fork;
}
