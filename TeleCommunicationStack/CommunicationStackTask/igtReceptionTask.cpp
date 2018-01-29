#include "igtReceptionTask.h"


igtReceptionTask::igtReceptionTask(int ID, igtInputQueue *inputQueue){
    if(inputQueue == NULL){
        return;
    }
    this->socketDescriptor = ID;
    this->inputQueueSave = inputQueue;
    this->received_frame_count = 0;

    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor)){
        //emit error(socket->error());
    }
    //socket->setReadBufferSize(1024);
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
}

void igtReceptionTask::run(){
    while(1){
        if (!socket->bytesAvailable()) {
            continue;
        }

        this->read();
        msleep(50);
    }
}

//! --------------------------------------------------------------------------
//!
//! \brief igtReceptionTask::read
//!
void igtReceptionTask::read(){
    this->received_frame_count += 1;
    QByteArray array = socket->read(1024);
    igtDatagram *datagram = new igtDatagram();
    datagram->decode(array);
    this->inputQueueSave->append(datagram);
}

//! --------------------------------------------------------------------------
//!
//! \brief igtReceptionTask::disc
//!
void igtReceptionTask::disc(){
    socket->deleteLater();
}

//! ---------------------------------------------------------------------------
//!
//! \brief igtReceptionTask::stop
//!
void igtReceptionTask::stop(){
    this->quit();
}
