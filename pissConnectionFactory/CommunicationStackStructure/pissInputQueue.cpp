#include "pissInputQueue.h"

pissInputQueue::pissInputQueue()
{
    clear();
}

pissInputQueue::~pissInputQueue()
{

}

void pissInputQueue::append(pissCDatagramme *datagramme){
    inputqueue.append(datagramme);
}

void pissInputQueue::clear(){
    inputqueue.clear();
}

void pissInputQueue::deleteFirstDatagramme(){
    inputqueue.pop_front();
}

pissCDatagramme* pissInputQueue::fetchFirstDatagramme(){
    return inputqueue.at(0);
}

int pissInputQueue::getLength(){
    return inputqueue.size();
}
