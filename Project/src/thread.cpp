#include "thread.h"
#include "pcb.h"
#include "os.h"
#include "schedule.h"


Thread::Thread(StackSize stack_size, Time time_slice) {
    OS::swLock();
    myPCB = new PCB(stack_size, time_slice, this);
    OS::swUnlock();
}

Thread::~Thread() {
    waitToComplete();
    OS::swLock();
    delete myPCB;
    OS::swUnlock();
}

void Thread::start() {
    if (myPCB->state == NEW) {
        myPCB->state = READY;
        myPCB->init_stack();
        Scheduler::put(myPCB);
    }
}

// t1 => t2.waitToComplete()
// t1 se blokira dok t2 ne zavrsi
void Thread::waitToComplete() {
    OS::swLock();
    if (myPCB->isActive()) {
        //upisi se u listu cekanja kod myPCB
        myPCB->wait_list.add(OS::running());
        OS::running()->state = BLOCKED;
        dispatch();
    }
    OS::swUnlock();
}

ID Thread::getId() {
    return myPCB->id;
}

ID Thread::getRunningId() {
    return OS::running()->id;
}

Thread* Thread::getThreadById(ID id) {
    return OS::getById(id)->thread;
}

void dispatch() {
    OS::dispatch();
}