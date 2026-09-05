# Kernel — RISC-V Multithreaded OS

A custom, library-style operating system kernel for the RISC-V (RV64IMA) architecture, implemented in C++.

The kernel provides threads, semaphores, time-sharing, and asynchronous preemption on timer/keyboard interrupts. It runs statically linked with user applications in a single address space, emulated via QEMU.

## Overview

This project implements a minimal but fully functional multithreaded kernel with time sharing, following an embedded-systems style design: the kernel and user application are compiled and statically linked into a single executable that shares one address space. Concurrent "processes" created by the application are lightweight threads managed entirely by the kernel — memory allocation, thread management, synchronization, and I/O are all implemented from scratch, without relying on any host OS or standard library services.

The kernel exposes its functionality through three layered interfaces:

```
  User's program
        │
   C++ OO API   ← object-oriented wrapper
        │
      C API     ← procedural syscall wrappers
        │
       ABI      ← binary syscall interface (software interrupt)
        │
      Kernel
        │
  HW access module
```

## Features

- **Memory Allocation** — `mem_alloc` / `mem_free`, block-based first-fit allocator with contiguous heap management
- **Thread Management** — `thread_create`, `thread_exit`, `thread_dispatch`, with kernel-managed stacks and full context switching
- **Synchronization** — `sem_open`, `sem_close`, `sem_wait` / `sem_signal`, plus batched `sem_wait_n` / `sem_signal_n` operations
- **Time Sharing & Preemption** — synchronous and asynchronous context switching on hardware timer interrupts, configurable time slices
- **Sleep / Wake** — `time_sleep` with an efficient sorted wake-time list for efficient thread resumption
- **Console I/O** — interrupt-driven, buffered `getc` / `putc` operations over a UART-style serial console interface
- **C++ Object-Oriented API** — `Thread`, `Semaphore`, `PeriodicThread`, and `Console` wrapper classes with `new` / `delete` operators transparently routed through the kernel allocator
- **Single-Processor Monolithic Kernel** — all kernel code runs in the same address space in privileged mode

## Tech Stack

- **Languages:** C++ (Kernel), RISC-V Assembly (Context switching/Traps)
- **Toolchain:** RISC-V GCC, Make
- **Emulation:** QEMU
- **Target:** RISC-V RV64IMA

## How to Run

Navigate to the project root and start the kernel using the Makefile:

```bash
cd RISCV-Kernel
make qemu
```

---

Open source — available for educational and systems programming learning. Feel free to fork, modify, and use for your own OS projects.
