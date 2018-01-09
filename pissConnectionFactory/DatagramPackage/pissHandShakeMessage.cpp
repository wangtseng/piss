#include "pissHandShakeMessage.h"

pissHandShakeMessage::pissHandShakeMessage()
{
}

pissHandShakeMessage::~pissHandShakeMessage(){

}


void pissHandShakeMessage::print(){
    qDebug()<<"print:";
    qDebug()<<"datatype:"<<this->getDataType();
           qDebug()<<"origineId:"<<this->getOrigineId();
          qDebug()<<"targetId:"<<this->getTargetId();
         qDebug()<<"timeStampes:"<<this->getTimeStampes();
        qDebug()<<"DLC:"<<this->getDLC();
       qDebug()<<"address:"<<this->getAddr();
      qDebug()<<"port:"<<this->getPort();
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::getDataType
//! \return
//!
int pissHandShakeMessage::getDataType(){
    int ret = this->dataType;
    return ret;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::setDataType
//! \param dataType
//!
void pissHandShakeMessage::setDataType(int dataType){
    this->dataType = dataType;
    return;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::getOrigineId
//! \return
//!
int pissHandShakeMessage::getOrigineId(){
    int ret = this->origineId;
    return ret;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::setOrigineId
//! \param origineId
//!
void pissHandShakeMessage::setOrigineId(int origineId){
    this->origineId = origineId;
    return;
}


//!----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::getTargetId
//! \return
//!
int pissHandShakeMessage::getTargetId(){
    int ret = this->targetId;
    return ret;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::setTargetId
//! \param targetId
//!
void pissHandShakeMessage::setTargetId(int targetId){
    this->targetId = targetId;
    return;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::getTimeStampes
//! \return
//!
int pissHandShakeMessage::getTimeStampes(){
    int ret = this->timeStampes;
    return ret;
}


//!----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::setTimeStampes
//! \param timeStampes
//!
void pissHandShakeMessage::setTimeStampes(int timeStampes){
    this->timeStampes = timeStampes;
    return;
}


//!-----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::getDLC
//! \return
//!
int pissHandShakeMessage::getDLC(){
    int ret = this->DLC;
    return ret;
}



//!------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::setDLC
//! \param DLC
//!
void pissHandShakeMessage::setDLC(int DLC){
    this->DLC = DLC;
    return;
}


//!-------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::getAddr
//! \return
//!
QString pissHandShakeMessage::getAddr(){
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
//! \brief HandShakeMessage::setAddr
//! \param address
//!
void pissHandShakeMessage::setAddr(QString address){
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
//! \brief HandShakeMessage::getPort
//! \return
//!
int pissHandShakeMessage::getPort(){
    int ret = this->port;
    return ret;
}


//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::setPort
//! \param port
//!
void pissHandShakeMessage::setPort(int port){
    this->port = port;
    return;
}


//!---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief HandShakeMessage::decodeDatagram
//! \param datagram
//!
void pissHandShakeMessage::decodeDatagram(pissCDatagramme *datagram){
    this->setDataType(datagram->getDataType());
    this->setOrigineId(datagram->getOrigineId());
    this->setTargetId(datagram->getTargetId());
    this->setTimeStampes(datagram->getTimeStampes());
    this->setDLC(datagram->getDLC());
    this->setAddr(datagram->getAddr());
    this->setPort(datagram->getPort());
}

