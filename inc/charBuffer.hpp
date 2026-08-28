//
// Created by os on 6/25/26.
//

#ifndef OS_PROJEKAT_BUFFER_H
#define OS_PROJEKAT_BUFFER_H

#include "sem.hpp"

class charBuffer {
public:
    void put(char c);
    char get();

    int available();

    bool isEmpty();

    bool putInterruptSafe(char c);
    charBuffer(int capacity = 1024);

    ~charBuffer();
    friend class consoleHandler;
private:
    int capacity;
    char* buffer;
    sem* mutex;
    sem* itemAvailable;
    sem* spaceAvailable;
    uint64 head = 0;
    uint64 tail = 0;
    int count = 0;
};



#endif //OS_PROJEKAT_BUFFER_H
