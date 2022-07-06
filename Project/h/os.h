/*
 * os.h
 *
 *  Created on: Jun 6, 2021
 *      Author: OS1
 */

#ifndef OS_H_
#define OS_H_

#include "types.h"


class PCB;
extern void tick();
class OS {
private:
	static int isIdle(PCB* pcb);
	static int threadTimeUp(PCB* pcb);
public:
	static PCB* running();
	static void init();
	static void restore();
	static void interrupt context_routine(...);
	static void dispatch();
	static void registerPcb(PCB* pcb);
	static void unregisterPcb(PCB* pcb);
	static PCB* getById(ID id);

	static void swLock();
	static void swUnlock();

};

#endif /* OS_H_ */
