//
// Created by os on 6/23/26.


#include  "../inc/sleepingList.hpp"
#include "../inc/Scheduler.hpp"

sleepingList::Node* sleepingList::head = nullptr;
sleepingList::Node* sleepingList::tail = nullptr;

int sleepingList::put(TCB *tcb, size_t time_left) {
    if (!tcb )return -1;
    tcb->setSleeping(true);
    Node* to_add = new Node(tcb);
    if (!to_add)return -1;
    if (!head) {
        head = tail = to_add;
        head->relative_time_left = time_left;
        return 0;
    }
    else {
        Node* curr = head;
        Node* prev = nullptr;
        uint64 sum = head->relative_time_left;
        uint64 prev_sum = 0;
        while (curr && sum < time_left) {
            prev = curr;
            curr = curr->next;
            prev_sum = sum;
            if (curr)sum += curr->relative_time_left;
        }
        if (!prev) {
            to_add->next = head;
            head = to_add;
            to_add->relative_time_left= time_left;
            if (to_add->next) {
                to_add->next->relative_time_left -= to_add->relative_time_left;
            }
            return 0;
        }

        //sada smo ili dosli do kraja ili imamo izmedju negde da ga stabvimo
        if (!curr) {

            tail->next = to_add;
            tail = to_add;
            tail->relative_time_left = time_left - prev_sum;
            return 0;
        }
        to_add->next = curr;
        prev->next = to_add;
        to_add->relative_time_left =time_left - prev_sum ;
        if (to_add->next) {
            to_add->next->relative_time_left -= to_add->relative_time_left;
        }
        return 0;
    }



}
void sleepingList::handleSleeping() {
    Node* curr = head;
    if (!curr)return;
    Node* prev;
    curr->relative_time_left--;
    if (curr->relative_time_left == 0) {
        while (curr && curr->relative_time_left == 0) {
            TCB* to_add = curr->data;
            to_add->setSleeping(false);
            Scheduler::put(to_add);
            prev = curr;
            curr = curr->next;

            delete(prev);
        }
    }
    head = curr;
    if (!head)tail = nullptr;
}
