//
// Created by os on 6/17/26.
//
#include "../inc/RiscV.hpp"
#include "../inc/MemoryAllocator.hpp"
#include "../inc/consoleHandler.hpp"
#include "../inc/syscall_c.hpp"
#include "../test/printing.hpp"
#include "../inc/Scheduler.hpp"
#include "../inc/TCB.hpp"
#include "../inc/sem.hpp"


void RiscV::handleSupervisorTrap() {
    volatile uint64 a0 = RiscV::read_a0();
    volatile uint64 arg1 = RiscV::read_a1();
    volatile uint64 arg2 = RiscV::read_a2();
    volatile uint64 arg3 = RiscV::read_a3();
    volatile uint64 arg4 = RiscV::read_a4();
    volatile uint64 sepc = RiscV::read_sepc();
    volatile uint64 sstatus = RiscV::read_sstatus();
    volatile uint64 scause = read_scause();

    if (scause == 0x8000000000000001UL) { //Prekid od Timer-a
        mc_sip(SIP_SSIP); // Očisti prekid
        Scheduler::handleSleeping();
        if (TCB::running == nullptr) {
            RiscV::write_sstatus(sstatus);
            TCB::dispatch();
            RiscV::write_sstatus(sstatus);
        }
        TCB::timeSliceShared++;

        if (TCB::timeSliceShared >= TCB::running->getTimeSlice()) {


            TCB::timeSliceShared = 0;
            TCB::dispatch();

            write_sstatus(sstatus);
            write_sepc(sepc);
        }
    }

    else if (scause == 0x8000000000000009UL) { //prekid od PLIC
       volatile uint64 interrupt_req = plic_claim();
        if (interrupt_req == CONSOLE_IRQ) {
            //prvo citamo sve sto ima da se procita
            consoleHandler::getInstance()->handleInterupt();
        }
        plic_complete(interrupt_req); //cak i kad je prekid stigao od neceg drugog na primer mis, onda ga cistimo da nastavimo dalje
        write_sstatus(sstatus);
        write_sepc(sepc);
    }
    else if (scause == 0x0000000000000009UL || scause == 0x0000000000000008UL) {//ecall (nije prekid)
        sepc += 4; // Inkrementiraj adresu da preskočiš ecall instrukciju
        volatile long ret_val;
        sem* handle;
        sem** s;

        switch (a0) {
            case MEM_ALLOC:
                void* volatile ptr;
                volatile size_t num_bytes;
                num_bytes = arg1 * MEM_BLOCK_SIZE;
                ptr = MemoryAllocator::getInstance()->kmalloc(num_bytes);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(ptr));
                break;
            case MEM_FREE:
                ret_val = MemoryAllocator::getInstance()->memfree((void*)arg1);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(ret_val));
                    break;
            case THREAD_CREATE:
                ret_val = TCB::createThread((TCB**) arg1, (void(*)(void*)) arg2, (void*) arg3, (uint64*) arg4);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(ret_val));
                break;
            case THREAD_DISPATCH:
                write_sstatus(sstatus);
                write_sepc(sepc);
                TCB::dispatch();
                break;
            case THREAD_EXIT:

                TCB::exit();
                write_sstatus(sstatus);
                write_sepc(sepc);
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(0));
                break;
            case TIME_SLEEP:


                ret_val = Scheduler::addSleeper(TCB::running, (time_t)arg1);


                TCB::dispatch();
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(ret_val));
                break;
            case SEM_OPEN:
                s = (sem**) arg1;
                if (!s)ret_val = -1;
            {
                    ret_val = sem::createSem(s, (int)arg2);
                }
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(ret_val));
                break;
            case SEM_CLOSE:
                handle = (sem*)arg1;
                if (handle == nullptr )ret_val = -1;
                else{

                    ret_val = handle->close();
                }
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(ret_val));
                break;
            case SEM_WAIT:
                handle = (sem*)arg1;
                if (handle == nullptr)ret_val = -1;
                else{
                    ret_val = handle->wait();

                }
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(ret_val));
                break;
            case SEM_WAIT_N:
                handle = (sem*)arg1;
                if (handle == nullptr)ret_val = -1;
                else{
                    ret_val = handle->sem_wait_n((uint64)arg2);

                }
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(ret_val));
                break;
            case SEM_SIGNAL:
                handle = (sem*)arg1;
                if (handle == nullptr)ret_val = -1;
                else{
                    ret_val = handle->signal();

                }
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(ret_val));
                break;
            case SEM_SIGNAL_N:
                handle = (sem*)arg1;
                if (handle == nullptr)ret_val = -1;
                else{
                    ret_val = handle->sem_signal_n((uint64)arg2);
                }
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(ret_val));
                break;
            case PUTC:
                consoleHandler::getInstance()->putchar((char)arg1);
                break;
            case GETC:
                char c;
                c = consoleHandler::getInstance()->getchar();
                __asm__ volatile ("sd %0, 80(s0)" : : "r"(c));
                break;
            default:

                break;
        }
       write_sstatus(sstatus);
       write_sepc(sepc);

        return ;
    }

    else if (scause == 0x0000000000000002UL) {
    printString("Nemate pravo koristiti tu instrukciju.\n");
    printString("Instukcija nadjena na adresi: ");
    printInt(sepc, 16, 0);
    printString("\n");
    consoleHandler::shutdown();
    // preskoči ilegalnu instrukciju i nastavi (ili ugasi thread)
    return;
}



    else {//unknown cause of interrupt
        printString("nepoznat razlog prekida\n");
        printString("sepc: ");
        printInt(sepc, 16, 0);
        printString("\n");
        printString("sstatus: ");
        printInt(sstatus, 16, 0);
        consoleHandler::shutdown();
        __asm__ volatile ("li t3, 0x100000");
        __asm__ volatile ("li t4, 0x5555");
        __asm__ volatile ("sw t4, 0(t3)");
    }
    return;
}

void RiscV::popSppSpie() {
    RiscV::mc_sstatus(RiscV::SSTATUS_SPP);
    RiscV::mc_sstatus(RiscV::SSTATUS_SPIE);
    __asm__ __volatile__("csrw sepc,ra ");
    __asm__ __volatile__("sret"); //da bi popovali sp
}