/*
 * event.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#include "event.h"
#include "kernelev.h"
#include "os.h"


Event::Event(IVTNo ivtNo) {
	//OS::swLock();
	MASK_INTS;
	myImpl = new KernelEv(ivtNo, this);
	//OS::swUnlock();
	UNMASK_INTS;
}

Event::~Event() {
	//OS::swLock();
	MASK_INTS;
	delete myImpl;
	//OS::swUnlock();
	UNMASK_INTS;
}


void Event::wait() {
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}


