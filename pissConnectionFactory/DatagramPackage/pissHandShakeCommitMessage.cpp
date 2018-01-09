#include "pissHandShakeCommitMessage.h"

pissHandShakeCommitMessage::pissHandShakeCommitMessage()
{
}

pissHandShakeCommitMessage::~pissHandShakeCommitMessage(){

}


//!----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::print
//!
void pissHandShakeCommitMessage::print(){
    //qDebug()<<this->dataType;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::getDataType
//! \return
//!
int pissHandShakeCommitMessage::getDataType(){
    int ret = this->dataType;
    return ret;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::setDataType
//! \param dataType
//!
void pissHandShakeCommitMessage::setDataType(int dataType){
    this->dataType = dataType;
    return;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::getOrigineId
//! \return
//!
int pissHandShakeCommitMessage::getOrigineId(){
    int ret = this->origineId;
    return ret;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::setOrigineId
//! \param origineId
//!
void pissHandShakeCommitMessage::setOrigineId(int origineId){
    this->origineId = origineId;
    return;
}


//!----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::getTargetId
//! \return
//!
int pissHandShakeCommitMessage::getTargetId(){
    int ret = this->targetId;
    return ret;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::setTargetId
//! \param targetId
//!
void pissHandShakeCommitMessage::setTargetId(int targetId){
    this->targetId = targetId;
    return;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::getTimeStampes
//! \return
//!
int pissHandShakeCommitMessage::getTimeStampes(){
    int ret = this->timeStampes;
    return ret;
}


//!----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::setTimeStampes
//! \param timeStampes
//!
void pissHandShakeCommitMessage::setTimeStampes(int timeStampes){
    this->timeStampes = timeStampes;
    return;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::getDLC
//! \return
//!
int pissHandShakeCommitMessage::getDLC(){
    int ret = this->DLC;
    return ret;
}



//!------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::setDLC
//! \param DLC
//!
void pissHandShakeCommitMessage::setDLC(int DLC){
    this->DLC = DLC;
    return;
}


//!-------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::getAddr
//! \return
//!
QString pissHandShakeCommitMessage::getAddr(){
//   QString addrs;
//   addrs.append(addr[0]);
//   addrs.append(".");
//   addrs.append(addr[1]);
//   addrs.append(".");
//   addrs.append(addr[2]);
//   addrs.append(".");
//   addrs.append(addr[3]);
//   return addrs;
    int addrInt[4];
    for(int i = 0; i < 4; i++){
        addrInt[i] = (int)addr[i];
    }
    QVector<QString> addrVector(4);
    for(int i = 0; i < 4; i++){
        addrVector[i] = QString::number(addrInt[i]);
    }
    QString addrs;
    addrs.append(addrVector[0]);
    addrs.append(".");
    addrs.append(addrVector[1]);
    addrs.append(".");
    addrs.append(addrVector[2]);
    addrs.append(".");
    addrs.append(addrVector[3]);
    return addrs;
}



//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::setAddr
//! \param address
//!
void pissHandShakeCommitMessage::setAddr(QString address){
    QStringList addrs = address.split(".");
    QString temp1 = addrs[0];
    this->addr[0] = (uchar)temp1.toInt();

    QString temp2 = addrs[1];
    this->addr[1] = (uchar)temp2.toInt();

    QString temp3 = addrs[2];
    this->addr[2] = (uchar)temp3.toInt();

    QString temp4 = addrs[3];
    this->addr[3] = (uchar)temp4.toInt();
}


//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::getPort
//! \return
//!
int pissHandShakeCommitMessage::getPort(){
    int ret = this->port;
    return ret;
}


//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::setPort
//! \param port
//!
void pissHandShakeCommitMessage::setPort(int port){
    this->port = port;
    return;
}


//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeCommitMessage::decodeDatagram
//! \param datagram
//!
void pissHandShakeCommitMessage::decodeDatagram(pissCDatagramme *datagram){
    this->setDataType(datagram->getDataType());
    this->setOrigineId(datagram->getOrigineId());
    this->setTargetId(datagram->getTargetId());
    this->setTimeStampes(datagram->getTimeStampes());
    this->setDLC(datagram->getDLC());
    this->setAddr(datagram->getAddr());
    this->setPort(datagram->getPort());
}
