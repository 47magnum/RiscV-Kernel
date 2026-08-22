//
// Created by os on 6/17/26.
//
#include  "../inc/syscall_c.hpp"
#include "../lib/hw.h"



void* mem_alloc(size_t num_bytes) {
    volatile uint64 num_blocks =  (num_bytes  + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1, %0" : : "r"(num_blocks));
    __asm__ volatile("mv a0, %0" : : "r"(1));
    __asm__ volatile("ecall");
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r"(ret_val));
    return (void*) ret_val;
}

int mem_free(void* ptr) {
    __asm__ volatile ("mv a1, %0" : : "r"(ptr));
    __asm__ volatile("mv a0, %0" : : "r"(0x02));
    __asm__ volatile("ecall");
    uint64 ret_val;
    __asm__ volatile ("mv %0, a0" : "=r"(ret_val));
    return (int)ret_val;
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    uint64* stack = new uint64[DEFAULT_STACK_SIZE / sizeof(uint64)];
    if (!stack) {
        mem_free(handle);
        return -1;
    }
    volatile uint64 function = (uint64)start_routine;
    volatile uint64 func_arg = (uint64)arg;
    volatile uint64 handler = (uint64)handle;

    __asm__ volatile ("mv a4, %0" : : "r"((uint64)stack));
    __asm__ volatile ("mv a3, %0" : : "r"(func_arg));
    __asm__ volatile ("mv a2, %0" : : "r"(function));
    __asm__ volatile ("mv a1, %0" : : "r"(handler));
    __asm__ volatile ("li a0, 0x11"); //kod za thread create
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int thread_exit() {
    __asm__ volatile ("li a0, 0x12");//kod operacije
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

void thread_dispatch() {
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
}

int time_sleep(time_t time) {
    __asm__ volatile ("mv a1, %0" : : "r"(time));
    __asm__ volatile ("li a0, 0x31");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int sem_open(sem_t *handle, unsigned init) {//ovde je jedino dupli pointer na sem
    __asm__ volatile ("mv a2, %0" : : "r"((int)init));
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x21");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int sem_close(sem_t handle) {
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x22");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int sem_wait(sem_t handle) {
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x23");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}


int sem_signal(sem_t id) {
    __asm__ volatile ("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x24");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int sem_wait_n(sem_t id, unsigned n) {
    __asm__ volatile ("mv a2, %0" : : "r"(n));
    __asm__ volatile ("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x25");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

int sem_signal_n(sem_t id, unsigned n) {
    __asm__ volatile ("mv a2, %0" : : "r"(n));
    __asm__ volatile ("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x26");
    __asm__ volatile ("ecall");
    int volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}


char getc() {
    __asm__ volatile ("li a0, 0x41");
    __asm__ volatile ("ecall");
    char volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r"(retval));
    return retval;
}

void putc(char c) {
    __asm__ volatile ("mv a1, %0" : : "r"(c));
    __asm__ volatile ("li a0, 0x42");
    __asm__ volatile ("ecall");
}
