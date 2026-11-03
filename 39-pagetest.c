// Assignment 10 Q3(c): pagetest.c
// Allocates one page, shows its page-table information, makes it
// read-only with setreadonly(), and then attempts to write to the
// protected page from a child process to show that a page fault occurs.
#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *page;
  int pid, channel[2], n;
  char marker;

  page = sbrk(4096);                  // allocate one page
  if(page == (char*)-1){
    printf(1, "pagetest: sbrk failed\n");
    exit();
  }
  page[0] = 42;                       // touch it so it is mapped

  printf(1, "pagetest: page information before setreadonly\n");
  pageinfo((int)page);

  if(setreadonly((int)page) < 0){
    printf(1, "pagetest: setreadonly failed\n");
    exit();
  }
  printf(1, "pagetest: page information after setreadonly\n");
  pageinfo((int)page);

  // A pipe detects an unexpected successful write instead of assuming a fault.
  if(pipe(channel) < 0){ printf(1, "pagetest: pipe failed\n"); exit(); }
  pid = fork();
  if(pid < 0){ printf(1, "pagetest: fork failed\n"); exit(); }
  if(pid == 0){
    close(channel[0]);
    // Re-apply protection in this process to test immediate TLB invalidation.
    if(setreadonly((int)page) < 0){ write(channel[1], "E", 1); exit(); }
    // child inherits the read-only page; writing must fault
    printf(1, "pagetest child: writing to the read-only page...\n");
    *(volatile char*)page = 43;                     // <-- page fault is expected here
    write(channel[1], "W", 1);
    printf(1, "pagetest child: write succeeded (unexpected!)\n");
    exit();
  }
  close(channel[1]);
  n = read(channel[0], &marker, 1);
  close(channel[0]);
  if(wait() != pid || n != 0 || page[0] != 42){
    printf(1, "pagetest: FAIL (unexpected child result)\n"); exit();
  }
  printf(1, "pagetest parent: protected write did not complete\n");
  printf(1, "pagetest parent: read of protected page still works: %d\n",
         page[0]);
  printf(1, "pagetest: PASS (see trap 14 err 7 above)\n");

  exit();
}
