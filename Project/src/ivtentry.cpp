/*
 * ivtentry.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#include "ivtentry.h"
#include "event.h"
#include <dos.h>


IVTEntry* IVTEntry::ivt[256];

IVTEntry::IVTEntry(Routine routine, IVTNo ivtNo) {
	MASK_INTS;

	ev = 0;
	IVTEntry::ivtNo = ivtNo;
	ivt[ivtNo] = this;

	_old_routine = getvect(ivtNo);
	setvect(ivtNo, routine);



	UNMASK_INTS;
}


IVTEntry::~IVTEntry() {
	MASK_INTS;

	setvect(ivtNo, _old_routine);
	_old_routine = 0;
	ivt[ivtNo] = 0;

	UNMASK_INTS;
}

void IVTEntry::signal_event() {
	if (ev) {
		ev->signal();
	}
}

void IVTEntry::registerEvent(Event *event) {
	MASK_INTS;
	ev = event;
	UNMASK_INTS;
}

void IVTEntry::unregister() {
	MASK_INTS;
	ev = 0;
	UNMASK_INTS;
}

void IVTEntry::old_routine() {
	_old_routine();
}

IVTEntry* IVTEntry::get(IVTNo ivtNo) {
	return ivt[ivtNo];
}

