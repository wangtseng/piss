#include "pissEncodingTask.h"


/**
 * @brief pissEncodingTask::pissEncodingTask
 * @param dataware
 * @param outputQueueManager
 * @param datagramAnalyser
 */
pissEncodingTask::pissEncodingTask(pissBaseDeDonnee *dataware, igtOutputQueueManager *outputQueueManager, igtDatagramAnalyser *datagramAnalyser)
{
    this->flag = true;
    this->dataware = dataware;
    this->outputQueueManager = outputQueueManager;
    this->datagramAnalyser  = datagramAnalyser;
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief pissEncodingTask::run
//!
void pissEncodingTask::run(){
    while (flag) {

        //! progress command
        if(this->dataware->getPatientHandling()->progressCommandsSequenceIsEmpty()){
            continue;
        }

        if(this->outputQueueManager->getOutputQueueManagerLength() > 0){
            igtDatagram *datagram = new igtDatagram();
            igtMotorParameter *temp = this->dataware->getPatientHandling()->fetchProgressMotorParameter();
            this->datagramAnalyser->transformMotorParameterToIgtDatagram(datagram, temp);
            this->outputQueueManager->setMessageToSendByTargetId(0, datagram);
            this->dataware->getPatientHandling()->removeFirstProgressMotorParameter();
        }

        //! rotate command
        if(this->dataware->getPatientHandling()->rotateCommandsSequenceIsEmpty()){
            continue;
        }

        if(this->outputQueueManager->getOutputQueueManagerLength() > 0){
            igtDatagram *datagram = new igtDatagram();
            igtMotorParameter *temp = this->dataware->getPatientHandling()->fetchRotateMotorParameter();

            this->datagramAnalyser->transformMotorParameterToIgtDatagram(datagram, temp);
            this->outputQueueManager->setMessageToSendByTargetId(0, datagram);
            this->dataware->getPatientHandling()->removeFirstRotateMotorParameter();
        }


        msleep(50);

    }
}

//! --------------------------------------------------------------------------------------------------------------------
void pissEncodingTask::stop(){
    this->flag = false;
}
