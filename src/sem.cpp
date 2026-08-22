//
// Created by os on 6/24/26.
//

#include "../inc/sem.hpp"


#include "../inc/Scheduler.hpp"
#include "../inc/List.hpp"
#include "../inc/printing.hpp"
#include "../lib/console.h"

int sem::signal() {
    if (closed)return -1;
    val++;
    semNode* top = blockedQueue.peakHead();
    if (!top)return 0; //niko ne ceka, resors oslobodjen
    // postoji neko ko ceka
    if (top->val <= val) {
        val -= top->val;
        top->tcb->setWaiting(false);
        Scheduler::put(top->tcb);
        semNode* curr = blockedQueue.getNext();
        delete curr;
        return 0;
    }
    return 0;
    // ako postoji cvor koji ceka ali nemamo resorse za njega, samo nastavljamo da cekamo jos signala dok ne mozemo da ga namirimo
}



int sem::wait() {
    if (closed)return -1;
    semNode* top = blockedQueue.peakHead();
    bool wasDeleted = false;
    if (!top) {
        //niko ne ceka
        if (val >0) {//niko ne ceka a imam dovoljno resorsa da ga primirim
            val --; //prolazim i ne blokiram se
            return 0;
        }
        else {
            //prazan je niz ali nema dovoljno resorsa pa cu samo da se blokiram
            semNode* newNode = new semNode(TCB::running, 1, &wasDeleted);
            blockedQueue.addElement(newNode);
            TCB::running->setWaiting(true);
            TCB::dispatch();
            if (wasDeleted)return -1;

        }
    }
    else {
        //neko ceka pa moram i ja da cekam (cim on ceka ja cu samo da se blokiram i to je to)
        semNode* newNode = new semNode(TCB::running, 1, &wasDeleted);
        blockedQueue.addElement(newNode);
        TCB::running->setWaiting(true);
        TCB::dispatch();
        if (wasDeleted)return -1;
    }
    return 0;
}


int sem::sem_wait_n(uint64 num) {
    if (closed)return -1;
    semNode* top = blockedQueue.peakHead();
    bool wasDeleted = false;
    if (!top) {
        //niko ne ceka
        if (val >= (int)num) {//niko ne ceka a imam dovoljno resorsa da ga primirim
            val-= num; //prolazim i ne blokiram se
            return 0;
        }
        else {
            //prazan je niz ali nema dovoljno resorsa pa cu samo da se blokiram
            semNode* newNode = new semNode(TCB::running, num, &wasDeleted);
            blockedQueue.addElement(newNode);
            TCB::running->setWaiting(true);
            TCB::dispatch();
            if (wasDeleted)delete newNode;


        }
    }
    else {
        //neko ceka pa moram i ja da cekam (cim on ceka ja cu samo da se blokiram i to je to)
        semNode* newNode = new semNode(TCB::running, num, &wasDeleted);
        blockedQueue.addElement(newNode);
        TCB::running->setWaiting(true);
        TCB::dispatch();
        if (wasDeleted)return -1;
    }
    return 0;
}

int sem::sem_signal_n(uint64 num) {
    if (closed)return -1;
    val += num;
    semNode* top = blockedQueue.peakHead();
    if (!top)return 0; //niko ne ceka, resors oslobodjen
    // postoji neko ko ceka
    while (top) {
        if (top->val <= val) {
            val -= top->val;
            top->tcb->setWaiting(false);
            Scheduler::put(top->tcb);
            semNode* curr = blockedQueue.getNext();
            delete curr;
            top = blockedQueue.peakHead();
        }
        else {
            return 0; //ne moze vise da ih prodje
        }
    }
    return 0;
}


sem::sem(int val, bool closed) : val(val), closed(false) {
    if (val < 0)return; //u sem open napravljam objekat
    blockedQueue = List<semNode>();
}

int sem::createSem(sem **handle, int initval) {
    *handle = new sem(initval, false);
    if (!handle)return -1;
    return 0;
}

int sem::close() {
    if (closed)return -1; //vec zatvoren
    closed = true;
    semNode *curr = blockedQueue.getNext();
    semNode* temp;
    while (curr) {
        TCB *data = curr->tcb;
        data->setWaiting(false);
        *(curr->deleted) = true;
        Scheduler::put(data);
        temp = curr;
        curr = blockedQueue.getNext();
        delete temp;
    }
    return 0;
}

sem::~sem() {
}
