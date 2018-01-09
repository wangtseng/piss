#include "igtOutputQueue.h"

igtOutputQueue::igtOutputQueue()
{

}
void igtOutputQueue::append(igtDatagram *sendMessage){
    this->outputQueue.append(sendMessage);
    return;
}

igtDatagram *igtOutputQueue::getLatestMessage(){
    igtDatagram *ret;
    mutex.lock();
    if(outputQueue.length() > 0){
        ret = this->outputQueue[0];
    }
    mutex.unlock();
    return ret;
}


//! delete latest frame : this->outputQueue.removeAt(0

void igtOutputQueue::deleteLatestFrame(){
    if(outputQueue.length() > 0){
        this->outputQueue.removeAt(0);
    }
}

int igtOutputQueue::getOutputQueueLength(){
    int ret;
    mutex.lock();
    ret = this->outputQueue.size();
    mutex.unlock();
    return ret;
}
