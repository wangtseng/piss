#include "igtDecodingTask.h"


igtDecodingTask::igtDecodingTask(pissBaseDeDonnee *dataware, igtInputQueueManager *inputQueueManager, igtDatagramAnalyser *datagramAnalyser)
{
    this->dataware = dataware;
    this->inputQueueManager = inputQueueManager;
    this->datagramAnalyser = datagramAnalyser;
    flag = true;

}

void igtDecodingTask::run(){
    while(flag){

        if(inputQueueManager->getLength() > 0){
            for(unsigned char cpt = 0; cpt < inputQueueManager->getLength(); cpt++){

                if(inputQueueManager->getDataArrayCountFromInputQueue(cpt) == 0){
                    continue;
                }

                igtDatagram *ret = inputQueueManager->getDataArrayFromInputQueue(cpt);
                this->datagramAnalyser->analyse(ret);
                inputQueueManager->removeDataArrayFromInputQueue(cpt);
            }
        }
        msleep(50);
    }
}

void igtDecodingTask::stop(){
    flag = false;
}
