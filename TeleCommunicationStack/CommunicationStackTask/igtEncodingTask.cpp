#include "igtEncodingTask.h"


/**
 * @brief igtEncodingTask::igtEncodingTask
 * @param dataware
 * @param outputQueueManager
 * @param datagramAnalyser
 */
igtEncodingTask::igtEncodingTask(pissBaseDeDonnee *dataware, igtOutputQueueManager *outputQueueManager, igtDatagramAnalyser *datagramAnalyser)
{
    this->flag = true;
    this->dataware = dataware;
    this->outputQueueManager = outputQueueManager;
    this->datagramAnalyser  = datagramAnalyser;
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief igtEncodingTask::run
//!
void igtEncodingTask::run(){
    while (flag) {

        //! progress command
        if(!this->dataware->getPatientHandling()->progressCommandsSequenceIsEmpty()){
            if(this->outputQueueManager->getOutputQueueManagerLength() > 0){
                igtDatagram *datagram = new igtDatagram();
                igtMotorParameter *temp = this->dataware->getPatientHandling()->fetchProgressMotorParameter();
                this->datagramAnalyser->transformMotorParameterToIgtDatagram(datagram, temp);
                this->outputQueueManager->setMessageToSendByTargetId(0, datagram);
                this->dataware->getPatientHandling()->removeFirstProgressMotorParameter();
            }
        }

        //! rotate command
        if(!this->dataware->getPatientHandling()->rotateCommandsSequenceIsEmpty()){
            if(this->outputQueueManager->getOutputQueueManagerLength() > 0){
                igtDatagram *datagram = new igtDatagram();
                igtMotorParameter *temp = this->dataware->getPatientHandling()->fetchRotateMotorParameter();
                this->datagramAnalyser->transformMotorParameterToIgtDatagram(datagram, temp);
                this->outputQueueManager->setMessageToSendByTargetId(0, datagram);
                this->dataware->getPatientHandling()->removeFirstRotateMotorParameter();
            }
        }

        //! injection command
        if(!this->dataware->getPatientHandling()->injectionCommandsSequenceIsEmpty()){
            if(this->outputQueueManager->getOutputQueueManagerLength() > 0){
                igtDatagram *datagram = new igtDatagram();
                igtContrastMediaPush *temp = this->dataware->getPatientHandling()->fetchInjectionCommand();
                this->datagramAnalyser->transformInjectionCommandToIgtDatagram(datagram, temp);
                this->outputQueueManager->setMessageToSendByTargetId(0, datagram);
                this->dataware->getPatientHandling()->removeFirstInjectionCommand();
            }
        }

        //! close session command
        if(!this->dataware->getPatientHandling()->closeSessionCommandSequenceIsEmpty()){
            if(this->outputQueueManager->getOutputQueueManagerLength() > 0){
                igtDatagram *datagram = new igtDatagram();
                igtCloseSessionMessage *temp = this->dataware->getPatientHandling()->fetchCloseSessionMessage();

                this->datagramAnalyser->transformCloseSessionMessageToIgtDatagram(datagram, temp);

                this->outputQueueManager->setMessageToSendByTargetId(0, datagram);
                this->dataware->getPatientHandling()->removeFirstCloseSessionMessage();
            }
        }

        msleep(20);

    }
}

//! --------------------------------------------------------------------------------------------------------------------
void igtEncodingTask::stop(){
    this->flag = false;
}


