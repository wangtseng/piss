#include "igtOutputQueueManager.h"


igtOutputQueueManager::igtOutputQueueManager(QObject *parent) : QObject(parent)
{
    this->outputQueueManagerLength = this->outputQueueManager.size();
    this->mutex = new QMutex();
}



void igtOutputQueueManager::addIgtOutputQueue(igtOutputQueue* outPutQueue){
    if(outPutQueue == NULL){
        return;
    }
    this->mutex->lock();
    outputQueueManager.append(outPutQueue);
    this->outputQueueManagerLength++;
    this->mutex->unlock();
}

void igtOutputQueueManager::setMessageToSendByTargetId(int ret,igtDatagram *sendMessage){
    this->mutex->lock();
    if(sendMessage == NULL || this->outputQueueManagerLength <=0 || ret >= this->outputQueueManagerLength){
        return;
    }
    outputQueueManager[ret]->append(sendMessage);
    this->mutex->unlock();
}

igtDatagram *igtOutputQueueManager::getLatestMessageByOutputQueueIndex(int index){
    igtDatagram *ret;
    this->mutex->lock();
    ret = this->outputQueueManager[index]->getLatestMessage();
    this->mutex->unlock();
    return ret;
}

void igtOutputQueueManager::deleteLatestMessageByOutputQueueIndex(int index){
    this->mutex->lock();
    if(index >= this->outputQueueManagerLength){
        return;
    }
    this->outputQueueManager[index]->deleteLatestFrame();
    this->mutex->unlock();
}

int igtOutputQueueManager::getLatestMessageLengthByOutputQueueIndex(int index){
    int ret;
    if(index >= this->outputQueueManagerLength){
        return -1;
    }
    this->mutex->lock();
    ret = outputQueueManager[index]->getOutputQueueLength();
    this->mutex->unlock();
    return ret;
}

int igtOutputQueueManager::getOutputQueueManagerLength(){
    int ret ;
    this->mutex->lock();
    ret = this->outputQueueManagerLength;
    this->mutex->unlock();
    return ret;
}
