// Assignment 10 Q2(c): vttest.c
// Allocates memory with sbrk(), passes different virtual addresses to
// vtop(), prints the corresponding physical addresses, and demonstrates
// the result for an unmapped virtual address.
#include "types.h"
#include "stat.h"
#include "user.h"

#define NPAGES 3

int
main(void)
{
  char *base;
  int i, pa, bad;

  base = sbrk(NPAGES * 4096);         // allocate three pages
  if(base == (char*)-1){
    printf(1, "vttest: sbrk failed\n");
    exit();
  }
  for(i = 0; i < NPAGES; i++)
    base[i * 4096] = (char)i;         // touch every page

  printf(1, "vttest: translations of mapped addresses\n");
  for(i = 0; i < NPAGES; i++){
    pa = vtop((int)(base + i * 4096));
    if(pa >= 0)
      printf(1, "va %x -> pa %x\n", (uint)(base + i * 4096), pa);
    else
      printf(1, "va %x -> not mapped\n", (uint)(base + i * 4096));
  }
  // an address part-way into a page: frame base + page offset
  pa = vtop((int)(base + 123));
  printf(1, "va %x -> pa %x\n", (uint)(base + 123), pa);

  // an address far beyond the allocation is unmapped
  bad = (int)(base + 100 * 4096);
  pa = vtop(bad);
  if(pa >= 0)
    printf(1, "va %x -> pa %x\n", (uint)bad, pa);
  else
    printf(1, "va %x -> not mapped (invalid/unmapped address)\n", (uint)bad);

  exit();
}
