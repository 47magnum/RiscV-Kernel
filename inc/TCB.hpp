//
// Created by os on 6/18/26.
//

#ifndef OS_PROJEKAT_TCB_H
#define OS_PROJEKAT_TCB_H
#include "../lib/hw.h"

class sleepingList;
class TCB {
    struct Context {
        uint64 ra;//gde treba da se vrati nit kad dodje do nje
        uint64 stackpointer; // gde je stek niti

    };
public:
    using Body = void (*)(void*);
    static TCB* create(Body body);

    static int createThread(TCB **handle, Body body, void *arg, uint64 *stackptr);

    TCB(Body body, void *arg, uint64 timeSlice, uint64 *stackptr);

    ~TCB();
    void setFinished(bool val) {
        isfinished = val;
    }
    bool getFinished() {
        return this->isfinished;
    }
    static TCB* running;
    static void yield();//u context sacuvamo ra i sp i onda pushujemo sve registre
    static uint64 timeSliceShared;
    friend class RiscV;
    static void threadWrapper();
    static int exit();
    TCB *createMain();

public:
    bool getSleeping() {
        return this->isSleeping;
    }
    void setSleeping(bool val) {
        this->isSleeping = val;
    }
    void setWaiting(bool val) {
        this->isWaiting = val;
    }
    bool getWaiting() {
        return this->isWaiting;
    }
    static void dispatch();

private:
    static void contextSwitch(Context* old, Context* new_context);
    uint64 getTimeSlice();
    Body body = nullptr;
    void* arg = nullptr;
    uint64* stack = nullptr;
    Context context;
    uint64 timeSlice;
    bool isfinished = false;
    bool isSleeping = false;
    bool isWaiting = false;

};

#endif //OS_PROJEKAT_TCB_H
