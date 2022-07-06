#ifndef _LIST_H_
#define _LIST_H_

#include "types.h"

class PCB;
class List {
private:
    struct Element {
        PCB *pcb;
        Element* next;
    };
    static Element* element(PCB* pcb);

    Element *head, *tail;
public:
    List();
    ~List();

    void add(PCB *pcb);
    PCB* poll(); // dohvata i uklanja prvi element liste
    PCB* remove(PCB *pcb); // uklanja zadati PCB iz lste
    PCB* get(ID id);

};

#endif
