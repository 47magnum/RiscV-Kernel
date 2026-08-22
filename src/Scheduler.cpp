//
// Created by os on 6/18/26.
//


#include "../inc/Scheduler.hpp"
List<TCB> Scheduler::readyQueue;
sleepingList* Scheduler::sleepQueue = nullptr;

TCB *Scheduler::get() {
    return readyQueue.getNext();
}

void Scheduler::put(TCB *data) {
    readyQueue.addElement(data);
}
uint64 Scheduler::getLength() {
    return readyQueue.getLength();

}
void Scheduler::handleSleeping() {
    Scheduler::sleepQueue->handleSleeping();
}

int Scheduler::addSleeper(TCB* data, time_t sleep_time) {
    if (!data)return -1;
    if (sleep_time == 0)return -1;
    return Scheduler::sleepQueue->put(data, sleep_time);
}

