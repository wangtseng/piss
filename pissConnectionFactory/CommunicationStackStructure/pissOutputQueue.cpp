#include "pissOutputQueue.h"

pissOutputQueue::pissOutputQueue(){
    cpt = 0;
    clear();
}

pissOutputQueue::~pissOutputQueue(){

}

void pissOutputQueue::append(pissCDatagramme *datagramme){
    oqMutex.lock();
    outputqueue.append(datagramme);
    cpt ++;
    oqMutex.unlock();
}

void pissOutputQueue::clear(){
    oqMutex.lock();
    outputqueue.clear();
    cpt = 0;
    oqMutex.unlock();
}

void pissOutputQueue::deleteFrontElement(){
    oqMutex.lock();
    outputqueue.pop_front();
    cpt --;
    oqMutex.unlock();
}

pissCDatagramme* pissOutputQueue::fetchFirstDatagramme(){
    pissCDatagramme *ret;
    oqMutex.lock();
    ret = outputqueue.at(0);
    oqMutex.unlock();
    return ret;
}

int pissOutputQueue::getLength(){

    int ret = 0;
    oqMutex.lock();
    ret = cpt;
    oqMutex.unlock();
    return ret;
}
