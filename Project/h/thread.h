#ifndef _thread_h_
#define _thread_h_

#include "types.h"

class OS;
class Thread {
public:
	 void start();
	 void waitToComplete();
	 virtual ~Thread();
	 ID getId();
	 static ID getRunningId();
	 static Thread * getThreadById(ID id);
protected:
	 friend class PCB;
	 friend class OS;
	 friend void wrapper();
	 Thread (StackSize stackSize = defaultStackSize, Time timeSlice =
	defaultTimeSlice);
	 virtual void run() {}
private:
	 PCB* myPCB;
};
void dispatch ();
#endif
