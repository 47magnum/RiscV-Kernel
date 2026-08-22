//
// Created by os on 6/18/26.
//
#include  "../inc/List.hpp"
#include "TCB.hpp"
#include "../inc/sleepingList.hpp"
#ifndef OS_PROJEKAT_SCHEDULER_H
#define OS_PROJEKAT_SCHEDULER_H


class Scheduler {
public:
    static void put(TCB* data);
    static uint64 getLength();

    static void handleSleeping();
    static int addSleeper(TCB *data, time_t sleep_time);
    static TCB* get();
private:
    static List<TCB> readyQueue;
    static sleepingList* sleepQueue;
};
#endif //OS_PROJEKAT_SCHEDULER_H
