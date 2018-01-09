#include "igtAddParameterRotateToOutputqueueTask.h"

igtAddParameterRotateToOutputqueueTask::igtAddParameterRotateToOutputqueueTask(pissBaseDeDonnee *dataware, igtOutputQueueManager *outputQueueManager, igtDatagramAnalyser *datagramAnalyser)
{
    this->flag = true;
    this->dataware = dataware;
    this->outputQueueManager = outputQueueManager;
    this->datagramAnalyser = datagramAnalyser;
}

void igtAddParameterRotateToOutputqueueTask::run(){
    while (flag) {
        if(this->dataware->getPatientHandling()->rotateCommandsSequenceIsEmpty()){
            continue;
        }

        if(this->outputQueueManager->getOutputQueueManagerLength() > 0){
            igtDatagram *datagram = new igtDatagram();
            igtMotorParameter *temp = this->dataware->getPatientHandling()->fetchRotateMotorParameter();
//            qDebug()<<"MotorType:"<<temp->getMotorType()<<"Symbol:"<<temp->getSymbol()<<"RPM:"<<temp->getRPM();

            this->datagramAnalyser->transformMotorParameterToIgtDatagram(datagram, temp);
            this->outputQueueManager->setMessageToSendByTargetId(0, datagram);
            this->dataware->getPatientHandling()->removeFirstRotateMotorParameter();
        }

        msleep(50);

    }
}

void igtAddParameterRotateToOutputqueueTask::stop(){
    this->flag = false;
}
