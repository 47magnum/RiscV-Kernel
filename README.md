# Kernel — RISC-V Multithreaded OS

A custom, library-style operating system kernel for the RISC-V (RV64IMA) architecture, implemented in C++.

The kernel provides threads, semaphores, time-sharing, and asynchronous preemption on timer/keyboard interrupts. It runs statically linked with user applications in a single address space, emulated via QEMU.

## Architecture & Data Flow

```text
  User's program
        │
   C++ OO API    ← Object-oriented wrappers
        │
     C API       ← Procedural syscall wrappers
        │
      ABI        ← Binary syscall interface (software interrupt)
        │
    Kernel
        │
 HW access module
```

## Features

- **Dynamic Memory Allocation:** Block-based first-fit allocator (mem_alloc/mem_free).
- **Thread Management:** Kernel-managed stacks, context switching, and preemption.
- **Synchronization:** Semaphores with standard wait/signal and batch operations.
- **Asynchronous Preemption:** Time-sharing driven by hardware timer interrupts.
- **Interrupt-Driven I/O:** Buffered serial console I/O.

## Tech Stack

- **Languages:** C++ (Kernel), RISC-V Assembly (Context switching/Traps).
- **Toolchain:** RISC-V GCC, Make.
- **Emulation:** QEMU.

## How to Run

Navigate to the project root and start the kernel using the Makefile:

```bash
cd RISCV-Kernel
make qemu
```
