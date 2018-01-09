#include "igtAddHelloMessageTask.h"

igtAddHelloMessageTask::igtAddHelloMessageTask(int index, igtOutputQueueManager *outputQueueManager)
{
    this->flag = true;
    this->index = index;
    this->outputQueueManager = outputQueueManager;
}

//! -----------------------------------------------------------------------------------------------------
//!
//! \brief igtAddHelloMessageTask::run
//!
void igtAddHelloMessageTask::run(){
    while (this->flag) {
        igtHelloMessage *helloMessage = new igtHelloMessage();
        helloMessage->setCount(this->index+1);
        helloMessage->setConnectionState(24);
        igtDatagram *datagram = helloMessage->transformHelloMessageToIgtDatagram();
        outputQueueManager->setMessageToSendByTargetId(this->index, datagram);

        sleep(4);
    }
    return;
}
