/*
 * evlist.h
 *
 *  Created on: Sep 2, 2021
 *      Author: OS1
 */

#ifndef EVLIST_H_
#define EVLIST_H_

class KernelEv;
class EvList{
	struct Elem{
		KernelEv* kernelEv;
		Elem* next;
		Elem(KernelEv* p,Elem *s=0){
			kernelEv = p;
			next = s;
		}};

		Elem *cur,*prev;
		Elem *first, *last;

		int n;
public:
		EvList();
		virtual ~EvList();

		int getSize();
		void emptyList();
		EvList& atFirst();
		EvList& atNext();
		int isCur();
		KernelEv* getCur();

		void add(KernelEv* p);
		EvList& deleteCur();

		int findKernelEv(KernelEv*p);
		EvList& deleteKernelEv(KernelEv* p);



};