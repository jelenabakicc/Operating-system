#include "pcb.h"
#include "os.h"
#include "schedule.h"
#include "thread.h"
#include <dos.h>

static int generator() {
    static int id = 0;

    return id++;
}

PCB::PCB(StackSize stack_size, Time time_slice, Thread* thread)
    :stack(NULL),
     time_slice(time_slice),
     stack_size(stack_size),
     thread(thread),
     state(NEW),
     lock(0),
	 sem_ret_val(0)
{
    id = generator();
    //dodati se u listu svih PCB-ova
    OS::registerPcb(this);
}


PCB::~PCB() {
    //izbrisati se iz liste PCB-ova
    OS::unregisterPcb(this);
    delete[] stack;
}

static void wrapper() {
    OS::running()->thread->run();

    OS::swLock();
    OS::running()->state = DONE;

    PCB* pcb = OS::running()->wait_list.poll();
    while(pcb != 0) {
        pcb->state = READY;
        Scheduler::put(pcb);
        pcb = OS::running()->wait_list.poll();
    }
    OS::swUnlock();
    dispatch();
}

void PCB::init_stack() {
    int n = stack_size / sizeof(int);
    stack = new int[n];

    // Postavljamo I bit na 1, da bi u startu dozvolili prekide da se desavaju
    // kada se nit pokrene.
    stack[n - 1] = 0x0200;
    stack[n - 2] = FP_SEG(wrapper);
    stack[n - 3] = FP_OFF(wrapper);
    // Registre stavimo da su u pocetku nule
    for (int i = n - 4; i >= n - 12; --i) {
        stack[i] = 0;
    }

    reg_ss = FP_SEG(stack);
    reg_sp = FP_OFF(&stack[n - 12]);
}

int PCB::isActive() {
    return state == READY || state == BLOCKED;
}