// Assignment 10 Q1(c): memtest.c
// Allocates memory with sbrk(), shows physical memory statistics,
// releases the memory, and shows the updated statistics.
#include "types.h"
#include "stat.h"
#include "user.h"

#define NPAGES 4                      // number of pages to allocate

int
main(void)
{
  char *mem;

  printf(1, "memtest: statistics before allocation\n");
  memstats();

  mem = sbrk(NPAGES * 4096);          // grow the heap by NPAGES pages
  if(mem == (char*)-1){
    printf(1, "memtest: sbrk failed\n");
    exit();
  }
  mem[0] = 1;                         // touch the first page
  printf(1, "memtest: statistics after sbrk(%d bytes)\n", NPAGES * 4096);
  memstats();

  if(sbrk(-(NPAGES * 4096)) == (char*)-1){   // release the memory again
    printf(1, "memtest: sbrk shrink failed\n");
    exit();
  }
  printf(1, "memtest: statistics after releasing memory\n");
  memstats();

  exit();
}
