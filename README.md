# OS Assignment 10: xv6 x86

This package contains a complete buildable xv6-public tree in `source/`,
all four required user programs, supplemental vmcheck, real execution
screenshots and serial output in `evidence/`, and the two-page `report.pdf`.

Base: https://github.com/mit-pdos/xv6-public
Commit: eeb7b415dbcb12cc362d0783e41c3d1f44066b17
This is 32-bit x86 xv6, selected because the assignment uses walkpgdir and usys.S.
It is not compatible with xv6-riscv without porting.
The upstream MIT license is included in source/LICENSE.

Clone this repository and build directly in `source/`. The exact tested ZIP is also retained at the repository root.

## Build and run

Install GCC/binutils supporting `-m32`, GNU make, Perl, and QEMU x86.
On a typical Debian/Ubuntu machine:

    sudo apt install build-essential gcc-multilib qemu-system-x86
    cd source
    make xv6.img fs.img
    make qemu-nox CPUS=1

At the xv6 `$` prompt, run:

    memtest
    vttest
    pagetest
    pagereplace
    vmcheck
    usertests

Exit QEMU with Ctrl-A, then X. For a new full usertests run, rebuild fs.img
from scratch: usertests creates a marker and otherwise refuses to run twice.

    rm -f fs.img
    make fs.img

## Changes

- kalloc.c: successful allocations, runtime frees, boot supply and locked snapshot.
  Boot free-list seeding is excluded from runtime frees so current = alloc - free
  is nonnegative. Available pages count the current free list. Statistics cover
  allocator-managed physical pages, not reserved kernel pages/all installed RAM.
- vm.c: read-only walkpgdir helpers, frame+offset translation, P/W/U inspection,
  PTE_W removal and CR3 reload. User calls reject kernel/out-of-process addresses.
- sysproc.c, syscall.c, syscall.h, defs.h, user.h, usys.S: four syscall paths.
- Makefile: four required demos and supplemental vmcheck in UPROGS.
- memtest.c / vttest.c / pagetest.c / pagereplace.c: required demonstrations.
- vmcheck.c: invalid-address and translation-offset checks.

Every assignment edit is identified by Assignment 10 comments.
`assignment10.patch` includes tracked-file changes; all five new user programs
are supplied separately in source/ (not duplicated inside that patch).

## Verified results

Built with GCC 11.4.0, QEMU 6.2.0, one CPU, 512 MB machine RAM, stock PHYSTOP
224 MB. All required demos, vmcheck and the complete built-in usertests passed.
The expected page-protection child fault is trap 14 / error 7, with parent read
still 42. The built-in suite emits intentional protection/OOM diagnostics and
finishes ALL TESTS PASSED. Serial logs retain the full run; screenshots are
pixel captures of the actual terminal session, trimmed only at unused edges.
The replacement summary image is a crop of the full Q4 screenshot.

Faults FIFO / LRU / Optimal:
- 3 frames: 15 / 12 / 9
- 4 frames: 10 / 8 / 8
- 5 frames: 9 / 7 / 7

All nine rows independently checked with a separate Python implementation.
The replacement program is a user-space simulation, not a kernel swap subsystem.
Physical addresses and cumulative counters vary with process activity.

## Service-access status

A new Google account was created as requested. Claude and Gemini sign-in hit
CAPTCHA challenges. Neither service generated or reviewed these sources.
No paid plan was started. No credentials are included in this package.
