/*
 * ivtentry.h
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "types.h"


#define PREPAREENTRY(ivtNo, old)							\
	void interrupt event_routine##ivtNo(...);				\
	IVTEntry ivtEntry##ivtNo(event_routine##ivtNo, ivtNo);	\
	void interrupt event_routine##ivtNo(...) {				\
		ivtEntry##ivtNo.signal_event();						\
		if (old) {											\
			ivtEntry##ivtNo.old_routine();					\
		}													\
		dispatch();											\
	}


class Event;
class IVTEntry {
private:
	static IVTEntry* ivt[256];
	IVTNo ivtNo;
	Routine _old_routine;
	Event *ev;
public:
	IVTEntry(Routine routine, IVTNo ivtNo);
	~IVTEntry();
	void signal_event();
	void registerEvent(Event* event);
	void unregister();
	void old_routine();
	static IVTEntry* get(IVTNo ivtNo);
};


#endif /* IVTENTRY_H_ */
