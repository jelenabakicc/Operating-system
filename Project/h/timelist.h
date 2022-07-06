/*
 * timelist.h
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#ifndef TIMELIST_H_
#define TIMELIST_H_

#include "types.h"

class PCB;
class Semaphore;
class TimeList {
private:
	TimeList() {}
	~TimeList() {}
	struct TimeElement {
		TimeElement(PCB* pcb, Semaphore* sem, Time wait_time):pcb(pcb), sem(sem), wait_time(wait_time), next(0) {}
		PCB *pcb;
		Semaphore* sem;
		Time wait_time;
		TimeElement *next;
	};
	static TimeElement* head;

public:
	static void insert(PCB *pcb, Semaphore* sem, Time wait_time);
	static void remove(PCB* pcb);
	static void reduce();
};


#endif /* TIMELIST_H_ */

