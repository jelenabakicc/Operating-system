/*
 * kernelev.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#include "kernelev.h"
#include "ivtentry.h"
#include "os.h"
#include "thread.h"
#include "pcb.h"
#include "schedule.h"

KernelEv::KernelEv(IVTNo ivtNo, Event *ev):ivtNo(ivtNo),val(0) {
	owner = OS::running();
	IVTEntry::get(ivtNo)->registerEvent(ev);
}

KernelEv::~KernelEv() {
	IVTEntry::get(ivtNo)->unregister();
}



void KernelEv::wait() {
	if (owner != OS::running()) {
		return;
	}

	MASK_INTS;
	if (val == 0) {

		owner->state = BLOCKED;

		dispatch();
	}

	val = 0;
	UNMASK_INTS;
}

void KernelEv::signal() {
	MASK_INTS;
	if (owner->state == BLOCKED) {
		owner->state = READY;
		Scheduler::put(owner);
	}
	val = 1;  //mozda ovo treba okruziti sa MASK_INTS i UNMASK_INTS!!!!!!!!!
	UNMASK_INTS;
}