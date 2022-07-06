#include "pcb.h"
#include "list.h"
#include "SCHEDULE.H"


List::List()
    : head(0),
      tail(0)
{

}

List::~List() {
    Element *el;
    while(head != 0) {
        el = head;
        head = head->next;
        delete el;
    }
}


List::Element* List::element(PCB* pcb) {
    Element* el = new Element();
    el->next = 0;
    el->pcb = pcb;
    return el;
}

void List::add(PCB *pcb) {
    Element *elem = element(pcb);

    if (head == 0) {
        head = tail = elem;
    } else {
        tail->next = elem;
        tail = elem;
    }
}

PCB* List::poll() {
    Element *el = head;
    PCB *pcb;
    if (!el) {
        return 0;
    }

    head = head->next;
    if (head == 0) {
        head = tail = 0;
    }
    pcb = el->pcb;
    delete el;
    return pcb;
}

PCB* List::remove(PCB *pcb) {
    Element *el = head;
    Element *prev = 0;

    for(; el && el->pcb != pcb; el = el->next) {
        prev = el;
    }

    // Nismo nasli, vracamo 0.
    if (el == 0) {
        return 0;
    }

    if (!prev) {
        head = head->next;
    } else {
        prev->next = el->next;
        if (el == tail) {
            tail = prev;
        }
    }
    if (head == 0) {
        head = tail = 0;
    }
    delete el;

    return pcb;
}

PCB* List::get(ID id) {
    Element *el = head;

    for(; el && el->pcb->id != id; el = el->next) {
    }

    if (el == 0) {
        return 0;
    }else {
        return el->pcb;
    }
}
