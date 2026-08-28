//
// Created by os on 6/25/26.
//
#include  "../inc/charBuffer.hpp"
#include "../inc/syscall_c.hpp"

charBuffer::charBuffer(int capacity):capacity(capacity){
    this->capacity = capacity;
    buffer = new char[capacity];
    sem_open(&mutex, 1);
    sem_open(&spaceAvailable, capacity);
    sem_open(&itemAvailable, 0);
}
charBuffer::~charBuffer() {
    delete[] buffer;
    delete itemAvailable;
    delete mutex;
    delete spaceAvailable;
}

void charBuffer::put(char c) {
    sem_wait(spaceAvailable);
    sem_wait(mutex);
    buffer[tail] = c;
    tail = (tail + 1) % capacity;
    count++;
    sem_signal(mutex);

    sem_signal(itemAvailable);
}

char charBuffer::get() {
    sem_wait(itemAvailable);
    sem_wait(mutex);
    char c = buffer[head];
    head = (head + 1) % capacity;
    count--;
    sem_signal(mutex);
    sem_signal(spaceAvailable);
    return c;
}

int charBuffer::available() {
    return count;
}

bool charBuffer::isEmpty() {
    return count == 0;
}

bool charBuffer::putInterruptSafe(char c) {
    if (count > capacity )return false;
    buffer[tail++] = c;
    tail = tail % capacity;
    count++;
    itemAvailable->signal();
    return true;
}