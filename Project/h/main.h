/*
 * main.cpp
 *
 *  Created on: Jun 6, 2021
 *      Author: OS1
 */
#include <iostream.h>

#include "os.h"
#include "thread.h"
#include "semaphor.h"
#include "event.h"
#include <stdlib.h>


PREPAREENTRY(9,0);
Event event(9);
int fleg;

class TestThread : public Thread{
public:
	~TestThread(){
		waitToComplete();
	}
	void run();
};

void TestThread::run(){
	event.registerT();
	Semaphore sleep(0);

	while (fleg<10){

		event.wait();

		asm{
						cli
						in      al, 61h         //; Send acknowledgment withou
						or      al, 10000000b   //;   modifying the other bits.
						out     61h, al         //;
						and     al, 01111111b   //;
						out     61h, al         //;
						mov     al, 20h         //; Send End-of-Interrupt signal
						out     20h, al         //;
						sti
					}

		MASK_INTS;
		cout << "SIGNALED THREAD" << Thread::getRunningId() << endl;
		int r = (rand()%100)+1;
		UNMASK_INTS;

		sleep.wait(r);

	}
}

#define N 10
int userMain(int argc, char**argv){
	TestThread threads[N];

	for(int i=0; i< N; i++){
		threads[i].start();

	}
	cout << "Sve niti pokrenute!";

	Semaphore sem_sleep(0);
	while(fleg<10){
		fleg++;
		sem_sleep.wait(20);
	}

	for (int j=0; j<N; j++){
		threads[j].waitToComplete();
	}



}

int main(int argc, char** argv) {
	OS::init();
	int ret_val = userMain(argc, argv);
	OS::restore();

	cout << "Happy end :)" << endl;
	return ret_val;
}