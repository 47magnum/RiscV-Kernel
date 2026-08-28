//
// Created by os on 6/26/26.
//


#include "../inc/consoleHandler.hpp"
#include "../inc/syscall_c.hpp"


consoleHandler* consoleHandler::instance = nullptr;

consoleHandler::consoleHandler() {

}
consoleHandler *consoleHandler::getInstance() {
    if (instance == nullptr) {
        consoleHandler::init();
    }
    return instance;
}
consoleHandler::~consoleHandler() {
    delete inputBuffer;
    delete outputBuffer;
}

void consoleHandler::init() {
    instance = new consoleHandler();
    instance->inputBuffer = new charBuffer(1024);
    instance->outputBuffer = new charBuffer(1024);
}

void consoleHandler::putchar(char c) {
    outputBuffer->put(c);
}

char consoleHandler::getchar() {
    return inputBuffer->get();
}

char consoleHandler::getOutputChar() {
    return outputBuffer->get();
}


void consoleHandler::handleInterupt() {
    uint8* volatile data_input_reg = (uint8*)CONSOLE_RX_DATA;
    uint8* volatile status_reg = (uint8*)CONSOLE_STATUS;
    while ((*status_reg & CONSOLE_RX_STATUS_BIT )> 0 ){
        char c = (*data_input_reg);
        inputBuffer->putInterruptSafe(c);
    }
}
void consoleHandler::shutdown() {
        while (!consoleHandler::getInstance()->outputBuffer->isEmpty()) {
            thread_dispatch();
        }
        __asm__ volatile ("li t3, 0x100000");
        __asm__ volatile ("li t4, 0x5555");
        __asm__ volatile ("sw t4, 0(t3)");
    }

