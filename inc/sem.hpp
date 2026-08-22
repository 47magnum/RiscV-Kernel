//
// Created by os on 6/24/26.
//

#ifndef OS_PROJEKAT_SEM_H
#define OS_PROJEKAT_SEM_H
#include "List.hpp"
#include "TCB.hpp"


class sem {
    struct semNode {
        TCB* tcb = nullptr;
        int val;
        bool* deleted; //pokazivac na mesto na steku gde se cuva da li je sem izbrisan
        semNode(TCB* thread, int val, bool* wasdeleted):tcb(thread), val(val), deleted(wasdeleted) {}
    };
    public:
    int wait();

    int sem_wait_n(uint64 num);

    int sem_signal_n(uint64 num);

    int signal();
    static int createSem(sem **handle, int initval);
    ~sem();
    int close();

private:
    sem(int val = 0, bool closed = false);
    int val;

    List<semNode> blockedQueue;
    bool closed;


};

#endif //OS_PROJEKAT_SEM_H
