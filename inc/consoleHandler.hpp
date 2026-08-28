//
// Created by os on 6/25/26.
//

#ifndef OS_PROJEKAT_CONSOLE_H
#define OS_PROJEKAT_CONSOLE_H
#include "charBuffer.hpp"

class consoleHandler {
    public:

    static consoleHandler *getInstance();

    ~consoleHandler();

    static void init();

    char getchar();

    char getOutputChar();

    void putchar(char c);
    void handleInterupt();

    static void shutdown();



private:
    charBuffer* outputBuffer;
    charBuffer* inputBuffer;
    consoleHandler();
    static consoleHandler* instance;

};


#endif //OS_PROJEKAT_CONSOLE_H
