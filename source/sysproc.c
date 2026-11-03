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

// Assignment 10 Q1: memstats() - display physical memory statistics.
int
sys_memstats(void)
{
  int total_alloc, total_free, avail;

  getmemstats(&total_alloc, &total_free, &avail);
  cprintf("total allocated pages: %d\n", total_alloc);
  cprintf("total freed pages: %d\n", total_free);
  cprintf("currently allocated pages: %d\n", total_alloc - total_free);
  cprintf("available physical memory pages: %d\n", avail);
  return 0;
}

// Assignment 10 Q2: vtop(va) - translate virtual to physical address.
// Returns the physical address, or -1 when va is not mapped.
int
sys_vtop(void)
{
  int va;
  uint pa;

  if(argint(0, &va) < 0 || (uint)va >= myproc()->sz ||
     (uint)va >= KERNBASE) // Assignment 10: reject kernel/out-of-process VAs
    return -1;
  if(v2paddr(myproc()->pgdir, (uint)va, &pa) < 0){
    cprintf("vtop: virtual address %x is not mapped\n", va);
    return -1;
  }
  return (int)pa;
}

// Assignment 10 Q3a: pageinfo(va) - display frame number and P/W/U flags.
int
sys_pageinfo(void)
{
  int va;
  uint frame, flags;

  if(argint(0, &va) < 0 || (uint)va >= myproc()->sz ||
     (uint)va >= KERNBASE) // Assignment 10: reject kernel/out-of-process VAs
    return -1;
  if(pageinfofor(myproc()->pgdir, (uint)va, &frame, &flags) < 0){
    cprintf("pageinfo: virtual address %x is not mapped\n", va);
    return -1;
  }
  cprintf("pageinfo: va=%x frame=%d P=%d W=%d U=%d\n",
          va, frame,
          (flags & PTE_P) ? 1 : 0,
          (flags & PTE_W) ? 1 : 0,
          (flags & PTE_U) ? 1 : 0);
  return 0;
}

// Assignment 10 Q3b: setreadonly(va) - make the page holding va read-only.
int
sys_setreadonly(void)
{
  int va;

  if(argint(0, &va) < 0 || (uint)va >= myproc()->sz ||
     (uint)va >= KERNBASE) // Assignment 10: reject kernel/out-of-process VAs
    return -1;
  return setreadonlypage(myproc()->pgdir, (uint)va);
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
