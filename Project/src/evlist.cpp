/*
 * evlist.cpp
 *
 *  Created on: Sep 2, 2021
 *      Author: OS1
 */
#include <stdio.h>
#include "os.h"
#include "EvList.h"



EvList::EvList()
	:	first(0),
		last(0),
		cur(0),
		prev(0),
		n(0)
{

}


EvList::~EvList(){
	while(first){
		Elem* old = first;
		first = first->next;
		delete old;
	}
	last = 0;
	cur = 0;
	prev = 0;
	n = 0;
}

int EvList::getSize(){
	return n;
}

void EvList::emptyList(){
	while(first){
		Elem* old = first;
		first = first->next;
		delete old;
	}

	last = 0;
	cur = 0;
	prev = 0;
	n = 0;

}


EvList& EvList::atFirst(){
	cur = first;
	prev = 0;
	return *this;
}

EvList& EvList::atNext(){
	prev = cur;
	if (cur){
		cur = cur->next;
	}
	return *this;
}

int EvList::isCur(){
	return (cur != 0);
}

KernelEv* EvList::getCur(){

	if (!cur) {
		MASK_INTS;
		cout << "Ne postoji tekuci element" << endl;
		UNMASK_INTS;
		return 0;
	}

	return cur->kernelEv;
}


void EvList::add(KernelEv* p){

	if (!first){
		last = first = new Elem(p);
	}
	else{
		last->next = new Elem(p);
		last = last->next;
	}
	//last=(!first? first:last->next)=new Elem(p);
	n++;
}


EvList& EvList::deleteCur(){
	if (!cur) {
		MASK_INTS;
		cout << "Ne postoji tekuci element" << endl;
		UNMASK_INTS;
	}
	else{

		Elem* old = cur;
		cur = cur->next;

		if (!prev){
			first = cur;
		}
		else{
			prev->next = cur;
		}
		//(!prev? first:prev->next)=cur;
		if (!cur) last = prev;
		old->next = 0;
		delete old;
		n--;
	}
	return *this;
}

int EvList::findKernelEv(KernelEv*p){
	for (Elem *tmp = first; tmp != 0; tmp = tmp->next){
		if (tmp->kernelEv == p) return 1;
	}
	return 0;
}

EvList& EvList::deleteKernelEv(KernelEv* p){

	cur = first;
	prev = 0;
	while (cur){
		if (cur->kernelEv != p){
			prev = cur;
			cur = cur->next;
		}
		else{
			Elem* old = cur;
			cur = cur->next;
			if (!prev) {
				first = cur;
			}
			else{
				prev->next = cur;
			}
			if (old == last){
				last = prev;
			}
			old->next = 0;
			delete old;

			n--;
		}
	}
}