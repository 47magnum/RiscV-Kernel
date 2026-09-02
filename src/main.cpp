#include "../inc/TCB.hpp"
#include "../inc/RiscV.hpp"
#include "../inc/syscall_c.hpp"
#include "../inc/consoleHandler.hpp"
#include "../inc/consoleReader.hpp"


extern void userMain();
void wrapper(void* v) {
userMain();
}
int main()
{
    RiscV::write_stvec((uint64) &RiscV::supervisorTrap);
    TCB* mainNit;
    thread_create(&mainNit, nullptr, nullptr);
    TCB::running = mainNit;
    consoleHandler::init();


    TCB* console_reader;
    thread_create(&console_reader, sendToMonitor, nullptr);
    TCB* userThread;
    thread_create(&userThread, wrapper, nullptr);
    RiscV::ms_sstatus(RiscV::SSTATUS_SIE);

    while(!userThread->getFinished()) {
        thread_dispatch();
    }
    consoleHandler::shutdown();

    __asm__ volatile ("li t3, 0x100000");
    __asm__ volatile ("li t4, 0x5555");
    __asm__ volatile ("sw t4, 0(t3)");
    return 0;
}
