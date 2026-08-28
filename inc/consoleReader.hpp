//
// Created by os on 6/26/26.
//

#ifndef OS_PROJEKAT_CONSOLEREADER_H
#define OS_PROJEKAT_CONSOLEREADER_H
#include "consoleHandler.hpp"
#include "../lib/hw.h"

void sendToMonitor(void* arg) {
    uint8* data_reg = (uint8*)CONSOLE_TX_DATA;
    uint8* status_reg = (uint8*)CONSOLE_STATUS;
    consoleHandler* handler = consoleHandler::getInstance();
    while (true) {
        char c = handler->getOutputChar();
        while ((*status_reg & CONSOLE_TX_STATUS_BIT) == 0);
        *data_reg = c;
    }
}


#endif //OS_PROJEKAT_CONSOLEREADER_H
