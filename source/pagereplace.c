// Assignment 10 Q4: pagereplace.c
// Implements FIFO, LRU and Optimal page replacement in user space.
// For every page reference it displays the current frames and whether
// the reference was a hit or a fault, prints the total page faults for
// each algorithm with 3 frames, and then builds a comparison table for
// 3, 4 and 5 frames.
#include "types.h"
#include "stat.h"
#include "user.h"

#define NREF   20
#define MAXFR  5

#define ALGO_FIFO 0
#define ALGO_LRU  1
#define ALGO_OPT  2

// Reference string required by the assignment.
static int ref[NREF] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};

static char *algoname[3] = {"FIFO", "LRU", "Optimal"};

// print a two-decimal percentage (xv6 printf has no %f)
static void
printratio(int num, int den)
{
  int pm = num * 10000 / den;         // per-myriad
  printf(1, "%d.%d%d%%", pm / 100, (pm / 10) % 10, pm % 10);
}

// next position >= pos at which page is referenced, or NREF if never
static int
nextuse(int page, int pos)
{
  int i;
  for(i = pos; i < NREF; i++)
    if(ref[i] == page)
      return i;
  return NREF;
}

// Run one algorithm with nframes frames.  When trace != 0, print the
// frames after every reference.  Returns the number of page faults.
static int
simulate(int algo, int nframes, int trace)
{
  int frames[MAXFR];
  int age[MAXFR];          // FIFO: insertion tick; LRU: last-use tick
  int npages = 0, faults = 0, i, j;

  for(i = 0; i < nframes; i++)
    frames[i] = -1;

  for(i = 0; i < NREF; i++){
    int page = ref[i];
    int hit = 0;

    for(j = 0; j < npages; j++)
      if(frames[j] == page){
        hit = 1;
        if(algo == ALGO_LRU)
          age[j] = i;      // LRU: record the newest use
        break;
      }

    if(!hit){
      faults++;
      if(npages < nframes){
        frames[npages] = page;
        age[npages] = i;
        npages++;
      } else {
        int victim = 0;
        for(j = 1; j < nframes; j++){
          if(algo == ALGO_OPT){
            // evict the page used farthest in the future (or never)
            if(nextuse(frames[j], i + 1) > nextuse(frames[victim], i + 1))
              victim = j;
          } else {
            // FIFO: oldest insertion; LRU: least recent use
            if(age[j] < age[victim])
              victim = j;
          }
        }
        frames[victim] = page;
        age[victim] = i;
      }
    }

    if(trace){
      printf(1, "ref %d: [ ", page);
      for(j = 0; j < nframes; j++){
        if(frames[j] < 0)
          printf(1, "- ");
        else
          printf(1, "%d ", frames[j]);
      }
      printf(1, "] %s\n", hit ? "hit" : "FAULT");
    }
  }
  return faults;
}

int
main(void)
{
  int algo, nf, faults, hits;


  printf(1, "pagereplace: reference string (%d references):\n", NREF);
  for(nf = 0; nf < NREF; nf++)
    printf(1, "%d ", ref[nf]);
  printf(1, "\n");

  // (b) full trace with 3 frames for each algorithm
  for(algo = 0; algo < 3; algo++){
    printf(1, "\n--- %s with 3 frames ---\n", algoname[algo]);
    faults = simulate(algo, 3, 1);
    printf(1, "%s: total page faults = %d\n", algoname[algo], faults);
  }

  // (c) comparison table for 3, 4 and 5 frames
  printf(1, "\n=== comparison table ===\n");
  printf(1, "frames algo     faults hits hit-ratio fault-ratio\n");
  for(nf = 3; nf <= 5; nf++){
    for(algo = 0; algo < 3; algo++){
      faults = simulate(algo, nf, 0);
      hits = NREF - faults;
      printf(1, "%d      %s  %d      %d   ", nf, algoname[algo],
             faults, hits);
      printratio(hits, NREF);
      printf(1, "     ");
      printratio(faults, NREF);
      printf(1, "\n");

    }
  }
  printf(1, "Optimal is best or tied at each frame count.\n");
  printf(1, "At 5 frames, LRU and Optimal tie with 7 faults.\n");

  exit();
}
