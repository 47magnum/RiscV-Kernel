//
// Created by os on 6/19/26.
//
#include  "../inc/TCB.hpp"
#include  "../inc/RiscV.hpp"
#include "../inc/Scheduler.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::timeSliceShared = 0;


void TCB::yield() {
    __asm__ __volatile__ ("li a0, 0x13");
    __asm__ __volatile__("ecall"); // skok u prekidnu rutinu gde vrsimo promenu konteksta
}

uint64 TCB::getTimeSlice() {
    return this->timeSlice;
}

void TCB::dispatch() {

    TCB* old = TCB::running;
    if (!old->getSleeping() && !old->getWaiting() && !old->getFinished())//ne proveravamo za getFinished jer cemo brisati ako je finished
    {
        Scheduler::put(old);
    }
    TCB* next = Scheduler::get();


    if (next == nullptr) {
        if (!old->getFinished()) {
            next = old;
        }
        else {
            return;
        }
    }
    if (next == old) {
        running = old;
        return;
    }
    running = next;
    if (next != old)contextSwitch(&old->context, &next->context);//&oldcontext u a0, &newContext u a1
}

int TCB::createThread(TCB** handle , Body body, void* arg, uint64* stackptr) {
  *handle = new TCB(body, arg, DEFAULT_TIME_SLICE, stackptr);
    if (!handle)return -1;
    return 0;
}
TCB::TCB(Body body, void* arg, uint64 timeSlice, uint64* stackptr) :
            body(body),
            arg(arg),
            stack(stackptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[(DEFAULT_STACK_SIZE/8)] : 0
                    }),
            timeSlice(timeSlice)
            {
    if (body != nullptr) {
        Scheduler::put(this);
    }
}

void TCB::threadWrapper() {
    RiscV::popSppSpie();//morala je da ne bude inline da bi mogli da se vratimo nazad na izvrsavanje odavde,
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}

int TCB::exit() {
    running->setFinished(true);
    TCB::dispatch();
    return 0;
}

TCB::~TCB() {
    if (stack) {
        delete[] stack;
    }
}
