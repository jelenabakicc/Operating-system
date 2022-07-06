#ifndef _IDLE_H_
#define _IDLE_H_

#include "thread.h"

class Idle : public Thread {
public:
    Idle():Thread(defaultStackSize, 1) {}
    void run() {
    	cout << "IDLE IS HERE :D!" <<endl;
        while(1);

    }
};

#endif
