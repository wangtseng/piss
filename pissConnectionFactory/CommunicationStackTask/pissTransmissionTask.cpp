#include "pissTransmissionTask.h"


pissTransmissionTask::pissTransmissionTask(int id, QVector<pissOutputQueue *> *oq, pissDevices* environment, SOCKET socketTransmission){
    this->id = id;
    this->oq = oq;
    this->devices = environment;
    this->transmissionSocket = socketTransmission;
    this->flag = true;

    frameCounter = 0;
//!    transmissionTimer = new QTimer();

//!    this->connect(this->transmissionTimer, SIGNAL(timeout()), this, SLOT(transfer()));
}

pissTransmissionTask::~pissTransmissionTask(){

}

void pissTransmissionTask::launch(){
    //this->transmissionTimer->start(20);
    this->start();
}

void pissTransmissionTask::run(){
    while(this->flag){
        outputMutex.lock();
        if(oq->size() > 0){
            //qDebug()<<"start transfer"<<id<<oq<<oq->at(0);
            if(oq->at(id)->getLength() > 0)
            {
                //qDebug()<<oq->at(id)->fetchFirstDatagramme()->getValue();
                //this->transmissionSocket->write(*(oq->at(id)->fetchFirstDatagramme()->getValue()));
                int iResult = ::send(this->transmissionSocket, this->oq->at(this->id)->fetchFirstDatagramme()->getValue(), MAX_PACKET_SIZE+1, 0);

                if(iResult == SOCKET_ERROR){
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(this->transmissionSocket);
                    WSACleanup();
                    exit(1);
                }

                qDebug()<<"....";
                oq->at(id)->deleteFrontElement();
                frameCounter ++;
            }
            else{
                qDebug()<<"no msg in the outputqueue"<<id;
            }
        }
        qDebug()<<"frameCount:"<<this->frameCounter;
        outputMutex.unlock();
        msleep(5);
    }
}

void pissTransmissionTask::stop(){
    this->flag = false;
}

