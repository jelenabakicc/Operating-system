/*
 * os.cpp
 *
 *  Created on: Jun 6, 2021
 *      Author: OS1
 */

#include <iostream.h>
#include "os.h"
#include "pcb.h"
#include <dos.h>
#include "list.h"
#include "SCHEDULE.h"
#include "idle.h"
#include "timelist.h"

enum ContextCall {
    HARDWARE, SOFTWARE
};

static void interrupt (*old_routine)(...) = 0;
static int counter;
static ContextCall call_type;
static List all_pcbs;
static PCB* running_pcb = 0;
static Idle idle;

// Main nit koja sadrzi PCB naseg glavnog programa.
static Thread* main_thread;


int OS::threadTimeUp(PCB* pcb) {
    return (pcb->lock == 0) && (pcb->time_slice != 0 && counter == pcb->time_slice);
}

int OS::isIdle(PCB* pcb) {
    return pcb == idle.myPCB;
}

// ...
// psw
// RET_ADDR
// ax
// bx
// cx
// ...
// bp
void interrupt OS::context_routine(...) {
	static int hss, hsp;
    if (call_type == HARDWARE) {
        ++counter;

        TimeList::reduce();
        tick();
        old_routine();

    }

    // Uslov, da vidimo da li nit treba da izgubi procesor.
    if ((call_type == SOFTWARE) || threadTimeUp(running_pcb)) {
    	// 1. zapamtimo gde smo stali na steku za nit1
    	asm {
    		mov hss, ss
			mov hsp, sp
    	}
    	running_pcb->reg_ss = hss;
    	running_pcb->reg_sp = hsp;

    	// Da li tekucu nit vracamo u Scheduler?
    	if (running_pcb->state == READY && !isIdle(running_pcb)) {
    		Scheduler::put(running_pcb);
    	}

    	if ((running_pcb = Scheduler::get()) == NULL) {
            running_pcb = idle.myPCB;
    	}

        // Postavljamo stek na novoizabranu nit.
    	hss = running_pcb->reg_ss;
    	hsp = running_pcb->reg_sp;
    	asm {
    		mov ss, hss
			mov sp, hsp
    	}
    	running_pcb->state = READY;
        // Resetujemo brojac za merenje vremena provedeno u procesoru
    	counter = 0;
    }

    // reset old value
    //if (call_type == HARDWARE){ asm int 60h;}
    call_type = HARDWARE;
}


void OS::registerPcb(PCB* pcb) {
    all_pcbs.add(pcb);
}

void OS::unregisterPcb(PCB* pcb) {
    all_pcbs.remove(pcb);
}

void OS::dispatch() {
	MASK_INTS;
    call_type = SOFTWARE;
    context_routine();
    UNMASK_INTS;
}

PCB* OS::running() {
    return running_pcb;
}

void OS::swLock() {
    running_pcb->lock++;
}

void OS::swUnlock() {
    running_pcb->lock--;
}

PCB* OS::getById(ID id) {
    return all_pcbs.get(id);
}

// seg off => seg * 16 + off => stvarna adresa
//
void OS::init() {
    counter = 0;
    call_type = HARDWARE;
    MASK_INTS;
    // Podesimo prekid.
    old_routine = getvect(0x08);
    setvect(0x08, OS::context_routine);

    // Podesimo idle nit.
    idle.myPCB->init_stack();
    idle.myPCB->state = READY;

    // ne moramo da pokrecemo start, jer taj stek vec postoji.
    main_thread = new Thread();
    running_pcb = main_thread->myPCB;
    running_pcb->state = READY;
    UNMASK_INTS;
}

void OS::restore() {
    MASK_INTS;
    setvect(0x08, old_routine);
    idle.myPCB->state = DONE;
    main_thread->myPCB->state = DONE;
    delete main_thread;
    UNMASK_INTS;
}
