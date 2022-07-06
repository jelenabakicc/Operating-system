/*
 * types.h
 *
 *  Created on: Jun 6, 2021
 *      Author: OS1
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <stdio.h>
#include <iostream.h>

#define MASK_INTS asm pushf; asm cli;
#define UNMASK_INTS asm popf;


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

typedef void interrupt (*Routine)(...);
typedef unsigned char IVTNo;



#endif /* TYPES_H_ */
