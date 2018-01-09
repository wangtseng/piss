#include "igtClient.h"

/**
 * @brief igtClient::igtClient
 * @param index
 * @param outputQueueManager
 * @param targetIpLineEdit
 * @param targetPortLineEdit
 * @param MB
 */
igtClient::igtClient(int mode, int index, igtOutputQueueManager *outputQueueManager,QString targetIpLineEdit,QString targetPortLineEdit,bool MB)
{
    this->mode = mode;
    this->index = index;
    this->outputQueueManager = outputQueueManager;
    this->flag = true;
    this->connectstate = false;
    this->targetIpLineEdit = targetIpLineEdit;
    this->targetPortLineEdit = targetPortLineEdit;

    this->connectMotivateOrBack = MB;

    this->localIP = this->ipDetect();

    this->timer = new QTimer();
    this->connect(this->timer, SIGNAL(timeout()), this, SLOT(write()));

    this->connectTcpsocket = new QTcpSocket();
    this->connectTcpsocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    this->connect(this->connectTcpsocket, SIGNAL(connected()), this, SLOT(sendHandShakeOrCommitMessage()));
}

//! --------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::~igtClient
//!
igtClient::~igtClient(){

}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::connectera
//!
void igtClient::connectera(){
    this->connectTcpsocket->connectToHost(QHostAddress(targetIpLineEdit),targetPortLineEdit.toInt());
}

//! ----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::wellConnected
//!
void igtClient::wellConnected(){
    this->timer->start(20);
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::sendHandShakeOrCommitMessage
//!
void igtClient::sendHandShakeOrCommitMessage(){   
    connectstate = true;

    if(this->connectMotivateOrBack == true){
        sendHandShakeMessage();
    }else if(this->connectMotivateOrBack == false){
        sendHandShakeCommitMessage();
    }
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::sendHandShakeCommitMessage
//!
void igtClient::sendHandShakeCommitMessage(){
    igtHandShakeCommitMessage *handShakeCommitMessage = new igtHandShakeCommitMessage();
    handShakeCommitMessage->setAddr(this->localIP);
    handShakeCommitMessage->setPort(10704);
    igtDatagram *datagram = handShakeCommitMessage->transformHandShakeCommitMessageToIgtDatagram();
    int iResult = this->connectTcpsocket->write(datagram->encode());
    if(iResult == 1024){
        wellConnected();
        emit connectionEstablished();
    }
}

void igtClient::sendHandShakeMessage(){
    igtHandShakeMessage *handShakeMessage = new igtHandShakeMessage();
    handShakeMessage->setAddr(this->localIP);
    handShakeMessage->setPort(10704);
    igtDatagram *datagram = handShakeMessage->transformHandShakeMessageToIgtDatagram();
    this->connectTcpsocket->write(datagram->encode());
}

void igtClient::write(){
    if(outputQueueManager->getOutputQueueManagerLength() > 0){
        if(outputQueueManager->getLatestMessageLengthByOutputQueueIndex(this->index) > 0){
            igtDatagram *ret = this->outputQueueManager->getLatestMessageByOutputQueueIndex(this->index);
            QByteArray temp = ret->encode();
            int iResult = this->connectTcpsocket->write(temp);
            this->outputQueueManager->deleteLatestMessageByOutputQueueIndex(this->index);
        }
    }
}

QString igtClient::ipDetect(){
    QString ret;

    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    for(int nIter = 0; nIter < list.count(); nIter++){
        if(!list[nIter].isLoopback()){
            if(list[nIter].protocol() == QAbstractSocket::IPv4Protocol && list[nIter] != QHostAddress(QHostAddress::LocalHost)){

                ret = list[nIter].toString();
                if(ret.contains("172")){
                    break;
                }
            }
        }
    }
    return ret;
}

//!----------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::getConnectState
//! \return
//!
bool igtClient::getConnectionState(){
    return this->connectstate;
}
