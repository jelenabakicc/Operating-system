/*
 * kernelev.h
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "types.h"
#include "list.h"

class Event;
class PCB;
struct KernelEv {

	KernelEv(IVTNo ivtNo, Event* ev);
	~KernelEv();

	PCB* owner;
	int val; // 0 - dogadjaj se nije desio, 1 - dogadjaj se desio
	IVTNo ivtNo;
	void wait();
	void signal();

};



#endif /* KERNELEV_H_ */
