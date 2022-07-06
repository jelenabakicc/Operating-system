#ifndef _PCB_H_
#define _PCB_H_

#include "types.h"
#include "list.h"

class Thread;

enum PCBState {
    BLOCKED, READY, NEW, DONE
};

struct PCB {
    PCB(StackSize stack_size, Time time_slice, Thread *thread);
    ~PCB();
    // Inicijalizuje stek niti
    void init_stack();
    int isActive();

    int *stack;
    StackSize stack_size;
    PCBState state;
    Time time_slice;
    ID id;
    Thread *thread;
    int reg_sp;
    int reg_ss;
    int lock;
    List wait_list;
    int sem_ret_val;
};


#endif

