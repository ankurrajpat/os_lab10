# OS Assignment 10: xv6 x86

The complete, buildable source tree, four assignment demonstrations, supplemental edge-case test, real QEMU screenshots and two-page report are in **OS_Assignment_10_tested.zip**. Download and extract that package, then follow its README. The individual changed source files, screenshots, patch and report are also uploaded here for convenient inspection. Browser-uploaded filenames have numeric prefixes; use the filenames inside the ZIP to build.

## Build and run

Inside the extracted package:

```sh
cd source
make
make qemu-nox CPUS=1 QEMUEXTRA='-m 512'
```

At the xv6 prompt run `memtest`, `vttest`, `pagetest`, `pagereplace`, `vmcheck` and `usertests`. Exit QEMU with Ctrl-A, then X. See the packaged README for a fresh usertests run and environment notes.

## Verification

All four required demos, `vmcheck` and the complete built-in `usertests` passed in QEMU. The expected protection-test child fault is trap 14 / error 7; the parent still reads 42. Full terminal output and clean-build output are included. Screenshots are actual execution captures, not generated terminal images.

Page-replacement faults (FIFO / LRU / Optimal):

| Frames | FIFO | LRU | Optimal |
|---|---:|---:|---:|
| 3 | 15 | 12 | 9 |
| 4 | 10 | 8 | 8 |
| 5 | 9 | 7 | 7 |

These nine counts were independently checked. Page replacement is a user-space simulation, not a kernel swap subsystem. The source is based on MIT's xv6-public, with its original LICENSE included in the package.

Exact package: 828,228 bytes. SHA-256:

```
491aa5ba8688a20b19a6c45e90b2dc7996e653d5c98620d19f901297c1231d2d
```

## Service-access status

Claude and Gemini sign-in hit CAPTCHA challenges. Neither service generated or reviewed these sources. This is an independently implemented and tested version. No paid plan was started, and no credentials are included.
