#include "../lib/hw.h"
#include "../inc/syscall_cpp.hpp"

int time_sleep(time_t time);

void* operator new(size_t n) {
    return mem_alloc(n); 
}
void* operator new[](size_t n) {
    return mem_alloc(n);
}
void operator delete(void *p) noexcept { 
    mem_free(p); 
}
void operator delete[](void *p) noexcept { 
    mem_free(p); 
}

Thread::Thread() {
    this->body = &(Thread::wrapper);
    this->arg = this;
}

Thread::Thread(void (*body)(void *), void *arg)
    : myHandle(nullptr), body(body), arg(arg) {}

Thread::~Thread() {}

int Thread::start() {
    if(arg == this) return thread_create(&myHandle, wrapper, arg);
    else return thread_create(&myHandle, body, arg);
}

void Thread::dispatch() { 
    thread_dispatch(); 
}
int Thread::sleep(time_t time) {
    return time_sleep(time); 
}
PeriodicThread::PeriodicThread(time_t period): period(period) {}

PeriodicThread::~PeriodicThread() {
    terminate(); 
}
void PeriodicThread::terminate() { 
    period = 0;
}
void PeriodicThread::run() {
    while (period > 0) {
        this->sleep(period);
        this->periodicActivation();
    }
}
Semaphore::Semaphore(unsigned init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}
int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

void Console::putc(char c) {
    ::putc(c);
}
char Console::getc() {
    return ::getc();
}
