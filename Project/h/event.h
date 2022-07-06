/*
 * event.h
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "ivtentry.h"

typedef unsigned char IVTNo;
class KernelEv;
class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	friend class IVTEntry;
	void signal(); 
private:
	KernelEv* myImpl;
};



#endif /* EVENT_H_ */
