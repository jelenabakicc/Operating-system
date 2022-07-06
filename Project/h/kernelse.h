/*
 * kernelse.h
 *
 *  Created on: Jun 20, 2021
 *      Author: OS1
 */

#ifndef KERNELSE_H_
#define KERNELSE_H_

#include "list.h"

struct KernelSem {
	KernelSem(int val);
	int val;
	List blocked_threads;
};


#endif /* KERNELSE_H_ */
