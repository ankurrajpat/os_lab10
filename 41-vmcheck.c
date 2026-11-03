// Assignment 10: supplemental invalid-address and TLB regression tests.
#include "types.h"
#include "stat.h"
#include "user.h"
#define CHECK(x) if(!(x)){printf(1,"vmcheck FAIL line %d\n",__LINE__);exit();}
int main(void){
  char *p=sbrk(4096); int pa;
  CHECK(p!=(char*)-1);
  pa=vtop((int)p); CHECK(pa>=0);
  CHECK(vtop((int)p+123)==pa+123);
  CHECK(vtop(-1)==-1); CHECK(vtop(0x80000000)==-1);
  CHECK(pageinfo(-1)==-1); CHECK(setreadonly(-1)==-1);
  CHECK(setreadonly(0x80000000)==-1);
  CHECK(vtop((int)sbrk(0))==-1);
  CHECK(sbrk(-4096)!=(char*)-1); CHECK(vtop((int)p)==-1);
  CHECK(pageinfo((int)p)==-1); CHECK(setreadonly((int)p)==-1);
  printf(1,"vmcheck: PASS (offset, kernel, negative, heap-end, freed page)\n");
  exit();
}
