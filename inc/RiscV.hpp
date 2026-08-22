//
// Created by os on 6/17/26.
//
#include "../lib/hw.h"

#ifndef OS_PROJEKAT_RISCV_H
#define OS_PROJEKAT_RISCV_H
class RiscV {



    enum maskSip
    {
        SIP_SSIP = (1 << 1),//supervisor software interrupt PENDING
        SIP_STIP = (1 << 5),//supervisor trap interrupt PENDING
        SIP_SEIP = (1 << 9),//supervisor extrenal interrupt PENDING
    };
    friend class TCB;
    static void write_a0_stack(uint64 val);
    static void write_a1_stack(uint64 val);

    static void popSppSpie(); // pop status previous priviledge
    //and status previous interrupt enable bits
    static uint64 read_scause();






    static uint64 read_stval();

    static void ms_sip(uint64 mask);
    // mask clear register sip
    static void mc_sip(uint64 mask);

    // read register sip
    static uint64 r_sip();

    // write register sip
    static void w_sip(uint64 sip);
    friend class sleepingList;
public :
    static void mc_sstatus(uint64 mask);

    static uint64 read_sstatus();

    enum maskSstatus

{
    SSTATUS_SIE = (1 << 1),
    SSTATUS_SPIE = (1 << 5),
    SSTATUS_SPP = (1 << 8),
};
    static void ms_sstatus(uint64 mask);

    static void init();
     static void initstvec();
    static void write_stvec(uint64 val);
    static void handleSupervisorTrap();

    static uint64 read_sepc();


    //read register a2
    static uint64 read_a2();

    //read register a3
    static uint64 read_a3();

    //read register a4
    static uint64 read_a4();
    // write register sepc
    static void write_sepc(uint64 sepc);

    static void write_a0(uint64 val);

    static uint64 read_stvec();

    static void supervisorTrap();
    static  inline uint64 read_a0();

    static uint64 read_a1();
    static void write_sstatus(uint64 val);

private:

};

inline uint64 RiscV::read_stvec()
{
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}
inline void RiscV::write_stvec(uint64 val) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(val));
}


inline uint64 RiscV::read_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline uint64 RiscV::read_a0 () {
    uint64 volatile returnValue;
    __asm__ volatile("mv %0, a0":"=r"(returnValue));
    return returnValue;
}

inline uint64 RiscV::read_a1 () {
    uint64 volatile returnValue;
    __asm__ volatile("mv %0, a1":"=r"(returnValue));
    return returnValue;
}

inline void RiscV::ms_sip(uint64 mask)
{
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sip(uint64 mask)//clearuje bitove date u masci (and) sa nulama te
{
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sip()
{
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline uint64 RiscV::read_sepc()
{
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void RiscV::write_sepc(uint64 sepc)
{
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}
inline void RiscV::write_a0(uint64 val) {
    __asm__ volatile ("mv a0, %0" : : "r"(val));
}

inline void RiscV::write_sstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));

}


inline uint64 RiscV::read_stval() {
    uint64  val;
    asm volatile("csrr %0, stval" : "=r"(val));
    return val;
}








inline uint64 RiscV::read_sstatus() {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void RiscV::ms_sstatus(uint64 mask)
{
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));

}
inline void RiscV::mc_sstatus(uint64 mask)
{
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}


inline uint64 RiscV::read_a2()
{
    uint64 volatile a2;
    __asm__ volatile ("mv %0, a2" : "=r"(a2));
    return a2;
}

inline uint64 RiscV::read_a3()
{
    uint64 volatile a3;
    __asm__ volatile ("mv %0, a3" : "=r"(a3));
    return a3;
}

inline uint64 RiscV::read_a4()
{
    uint64 volatile a4;
    __asm__ volatile ("mv %0, a4" : "=r"(a4));
    return a4;
}


#endif //OS_PROJEKAT_RISCV_H
