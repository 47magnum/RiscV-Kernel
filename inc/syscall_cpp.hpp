//
// Created by os on 6/18/26.
//

#ifndef OS_PROJEKAT_SYSCALL_CPP_H
#define OS_PROJEKAT_SYSCALL_CPP_H


#include "../lib/hw.h"
#include "../inc/syscall_c.hpp"



void *operator new(size_t n);

void *operator new[](size_t n);

void operator delete(void *p) noexcept;

void operator delete[](void *p) noexcept;



class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);

protected:
    Thread ();
    virtual void run () {}
    static void wrapper(void* argument) {
        ((Thread*)argument)->run();
    }
private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;
};




class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};


class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);

    ~PeriodicThread();
    virtual void periodicActivation () {}
    virtual void run() override;
private:
    time_t period;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};
#endif