/*
 * timelist.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#include "pcb.h"
#include "os.h"
#include "semaphor.h"
#include "timelist.h"
#include "types.h"


TimeList::TimeElement* TimeList::head = 0;

void TimeList::insert(PCB *pcb, Semaphore* sem, Time wait_time) {
	TimeElement *el = new TimeElement(pcb, sem, wait_time);
	TimeElement *it = head;
	TimeElement *prev = 0;

	while(it && it->wait_time <= el->wait_time) {
		prev = it;
		it = it->next;
		el->wait_time -= prev->wait_time;
	}

	if (!prev) {
		head = el;
	} else {
		prev->next = el;
	}

	el->next = it;
	if (it) {
		it->wait_time -= el->wait_time;
	}
}

void TimeList::remove(PCB *pcb) {
	MASK_INTS;
	TimeElement *it = head;
	TimeElement *prev = 0;

	while(it && it->pcb != pcb) {
		prev = it;
		it = it->next;
	}

	if (it) {
		if (!prev) {
			head = head->next;
		} else {
			prev->next = it->next;
		}
		if (it->next) {
			it->next->wait_time += it->wait_time;
		}
		delete it;
	}
	UNMASK_INTS;
}

void TimeList::reduce() {
	if (!head) {
		return;
	}

	head->wait_time--;
	while(head && !head->wait_time) {
		TimeElement *el = head;
		head = head->next;
		el->sem->timeListUnblock(el->pcb);
//		cout << "Timed unblock ID: " << el->pcb->id << endl;
		delete el;
	}
}



