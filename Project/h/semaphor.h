/*
 * semaphor.h
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#ifndef _semaphor_h_
#define _semaphor_h_
typedef unsigned int Time;

class KernelSem;
class PCB;
class Semaphore {
public:
	Semaphore (int init=1);
	virtual ~Semaphore ();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const; // Returns the current value of the semaphore
private:
	friend class TimeList;
	void timeListUnblock(PCB *pcb);
	KernelSem* myImpl;
};
#endif
