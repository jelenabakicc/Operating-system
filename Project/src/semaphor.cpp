/*
 * semaphor.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#include "semaphor.h"
#include "os.h"
#include "pcb.h"
#include "kernelse.h"
#include "SCHEDULE.h"
#include "thread.h"
#include "timelist.h"
#include "types.h"

Semaphore::Semaphore(int val) {
	OS::swLock();
	myImpl = new KernelSem(val);
	OS::swUnlock();
}

Semaphore::~Semaphore() {
	OS::swLock();
	PCB *pcb = myImpl->blocked_threads.poll();
	while(pcb) {
		pcb->state = READY;
		Scheduler::put(pcb);
		pcb = myImpl->blocked_threads.poll();
	}
	delete myImpl;
	OS::swUnlock();
}



void Semaphore::timeListUnblock(PCB* pcb) {
	myImpl->blocked_threads.remove(pcb);
	myImpl->val++;
	pcb->sem_ret_val = 0;
	pcb->state = READY;
	Scheduler::put(pcb);
}

int Semaphore::wait(Time maxTimeToWait) {
	//OS::swLock();
	MASK_INTS;
	OS::running()->sem_ret_val = 1;
	// blokiramo se
	if (--myImpl->val < 0) {
		if (maxTimeToWait > 0) {
			TimeList::insert(OS::running(), this, maxTimeToWait);
//			cout << "Timed block ID: " << OS::running()->id << endl;
		}
		myImpl->blocked_threads.add(OS::running());
		OS::running()->state = BLOCKED;
		dispatch();
	}

	//OS::swUnlock();
	UNMASK_INTS;
	return OS::running()->sem_ret_val;
}


void Semaphore::signal() {
	//OS::swLock();
	MASK_INTS;
	if (myImpl->val++ < 0) {
		PCB* pcb = myImpl->blocked_threads.poll();
		TimeList::remove(pcb);
		pcb->state = READY;
		Scheduler::put(pcb);
	}

	//OS::swUnlock();
	UNMASK_INTS;
}


int Semaphore::val() const {
	return myImpl->val;
}