#include "igtInputQueueManager.h"

igtInputQueueManager::igtInputQueueManager(){
    printTimer = new QTimer();
    //this->connect(printTimer, SIGNAL(timeout()), this, SLOT(print()));
    //printTimer->start(10);
}

int igtInputQueueManager::getLength(){
    mutex.lock();
    int ret = igtInputDataware.size();
    mutex.unlock();
    return ret;
}

int igtInputQueueManager::getDataArrayCountFromInputQueue(int i){
    mutex.lock();
    int ret = igtInputDataware.at(i)->getLength();
    mutex.unlock();
    return ret;
}

igtDatagram *igtInputQueueManager::getDataArrayFromInputQueue(int i){
    mutex.lock();
    igtDatagram *ret = igtInputDataware.at(i)->getLatestArray();
    mutex.unlock();
    return ret;
}

void  igtInputQueueManager::removeDataArrayFromInputQueue(int i){
    igtInputDataware.at(i)->deleteLatestArray();
}

void igtInputQueueManager::addIgtInputQueue(igtInputQueue* inputQueue){
    igtInputDataware.append(inputQueue);
}

void igtInputQueueManager::print(){
    if(igtInputDataware.size()>0){
        //qDebug()<<"---------igtInputQueueManager---------";
        for(unsigned char cpt = 0; cpt < igtInputDataware.size(); cpt++){
            igtInputDataware.at(cpt)->print();
        }
        //qDebug()<<"_________igtInputQueueManager_________";
    }
}
