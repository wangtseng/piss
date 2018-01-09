#include "pissOutputInformationEncoder.h"

pissOutputInformationEncoder::pissOutputInformationEncoder(QVector<pissOutputQueue *> *outputQueueManager,
                                                           pissDevices *NetworkEnvironment,
                                                           pissDatagrammeAnalyser *datagrammeAnalyser)
{

    this->outputQueueManager = outputQueueManager;
    this->networkEnvironment = NetworkEnvironment;
    this->datagrammeAnalyser = datagrammeAnalyser;
    this->flag = true;
}

pissOutputInformationEncoder::~pissOutputInformationEncoder()
{

}

void pissOutputInformationEncoder::run(){
    return;
}

void pissOutputInformationEncoder::stop(){
    return;
}
