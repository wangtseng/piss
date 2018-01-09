#include "pissReceptionTask.h"

/**
 * @brief pissHandShake::pissHandShake
 * @param SD
 * @param datagrammeAnalyser
 * @param inputQueueManager
 * @param networkEnvironment
 */
pissReceptionTask::pissReceptionTask(int id,
                             SOCKET SD,
                             pissDatagrammeAnalyser *datagrammeAnalyser,
                             QVector <pissInputQueue*> *inputQueueManager,
                             pissDevices *networkEnvironment)
{
    flag = true;

    this->id = id;
    this->sd = SD;
    this->blockSize = 0;
    this->AIM = 0;
    this->transType = 0;
    this->DLC = 0;
    this->bytesReceived = 0;
    this->totalBytes = 0;
    this->datagrammeAnalyser = datagrammeAnalyser;
    this->inputQueueManager = inputQueueManager;
    this->networkEnvironment = networkEnvironment;

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissHandShake::~pissHandShake
//!
pissReceptionTask::~pissReceptionTask(){

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissHandShake::run
//!
void pissReceptionTask::run(){
    int count = 0;
    while (flag) {

        char chunk[MAX_PACKET_SIZE+1];
        int nRecv = ::recv(this->sd, chunk, MAX_PACKET_SIZE+1, 0);

        if(nRecv > 0){

            chunk[MAX_PACKET_SIZE] = '\0';
            pissCDatagramme *datagramme = new pissCDatagramme();
            datagramme->setValue(chunk);

            count++;

            this->mutexInputqueueManager.lock();
            this->inputQueueManager->at(this->id)->append(datagramme);
            this->mutexInputqueueManager.unlock();
            qDebug()<<"message arrived:"<<nRecv<<count;
        }

    }
    msleep(20);
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissHandShake::lecture
//!
//void pissReceptionTask::lecture()
//{
//    qDebug()<<"new message arrive";
//    QByteArray input = this->soc->readAll();
//    CDatagramme *datagramme = new CDatagramme();
//    datagramme->setValue(&input);

//    this->mutexInputqueueManager.lock();
//    this->inputQueueManager->at(this->id)->append(datagramme);
//    this->mutexInputqueueManager.unlock();
//}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissHandShake::disconnect
//!
//void pissReceptionTask::disconnect()
//{
//    this->soc->deleteLater();
//}
