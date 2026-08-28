//
// Created by os on 6/18/26.
//

#include   "../lib/hw.h"
#ifndef OS_PROJEKAT_LIST_H
#define OS_PROJEKAT_LIST_H


template <typename T>
class List {

public:
    List():head(nullptr), tail(nullptr){}
    void  addElement(T* data);

    void removeElement(T *data);
    uint64 getLength();



    struct Node {
        T* data;
        Node* next;
        Node(T* data):data(data),next(nullptr){}
    };
    T* getNext();

    T* peakHead();
private:
    Node* head;
    Node* tail;
};


//
// Created by os on 6/18/26.
//


template<typename T>
T* List<T>::getNext() {
    if (head == nullptr)return nullptr;
    T* data;
    Node* curr = head;
    if (head == tail) {
        data = curr->data;
        head = tail = nullptr;
    }
    else {
        head = head->next;
        data = curr->data;
    }
    delete curr;
    return data;
}

template<typename T>
void  List<T>::addElement(T* data) {
    Node* newNode = new Node(data);
    if (head) {
        tail->next = newNode;
        tail = newNode;
    }
    else {
        head = tail = newNode;

    }
}
template<typename T>
uint64 List<T>::getLength() {
    uint64 count = 0;
    Node* curr = head;
    while (curr) {
        curr = curr->next;
        count++;
    }
    return count;
}

template <typename T>
T* List<T>::peakHead() {
    if (head)return head->data;
    return nullptr;

}
#endif //OS_PROJEKAT_LIST_H
