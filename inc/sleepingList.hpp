//
// Created by os on 6/23/26.
//

#ifndef OS_PROJEKAT_SLEEPINGLIST_H
#define OS_PROJEKAT_SLEEPINGLIST_H

#include "../lib/hw.h"
class TCB;

class sleepingList {
    struct Node {
        TCB* data;
        Node* next;
        uint64 relative_time_left;
        Node(TCB* data):data(data),next(nullptr){}
    };
public :
    static int put(TCB *data, size_t time_left) ;
    static void handleSleeping();
    friend class TCB;
private:
    static Node* head;
    static Node* tail;
};


#endif //OS_PROJEKAT_SLEEPINGLIST_H
