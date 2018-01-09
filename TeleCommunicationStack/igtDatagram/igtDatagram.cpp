#include "igtDatagram.h"
#include <QDebug>

igtDatagram::igtDatagram()
{
    this->body.resize(1014);
}

unsigned int igtDatagram::getDataType(){
    unsigned int ret = this->dataType;
    return ret;
}

void igtDatagram::setDataType(unsigned int dataType){
    this->dataType = dataType;
}

unsigned int igtDatagram::getTargetId(){
    unsigned int ret = this->targetId;
    return ret;
}

void igtDatagram::setTargetId(unsigned int targetId){
   this->targetId = targetId;
}

unsigned int igtDatagram::getOrigineId(){
    unsigned int ret = this->origineId;
    return ret;
}

void igtDatagram::setOrigineId(unsigned int origineId){
    this->origineId = origineId;
}

unsigned int igtDatagram::getTimeStampes(){
    unsigned int ret = this->timeStampes;
    return ret;
}

void igtDatagram::setTimeStampes(unsigned int timeStampes){
    this->timeStampes = timeStampes;
}

unsigned int igtDatagram::getDLC(){
    unsigned int ret = this->DLC;
    return ret;
}

void igtDatagram::setDLC(unsigned int DLC){
    this->DLC = DLC;
}

QByteArray igtDatagram::getIgtDatagramBody(){
    QByteArray ret;
    ret = this->body;
    return ret;
}

void igtDatagram::setIgtDatagramBody(QByteArray igtDatagramBody){
    this->body.replace(0, 4, igtDatagramBody);
}

QByteArray igtDatagram::encode(){
    QByteArray encodeByteArray;
    encodeByteArray.resize(1024);

    encodeByteArray[0] = (uchar)(0x000000ff&this->getDataType());
    encodeByteArray[1] = (uchar)((0x0000ff00&this->getDataType())>>8);
    encodeByteArray[2] = (uchar)(0x000000ff&this->getOrigineId());
    encodeByteArray[3] = (uchar)(0x000000ff&this->getTargetId());
    encodeByteArray[4] = (uchar)(0x000000ff&this->getTimeStampes());
    encodeByteArray[5] = (uchar)(0x0000ff00&this->getTimeStampes()>>8);
    encodeByteArray[6] = (uchar)(0x00ff0000&this->getTimeStampes()>>16);
    encodeByteArray[7] = (uchar)(0xff000000&this->getTimeStampes()>>24);
    encodeByteArray[8] = (uchar)(0x000000ff&this->getDLC());
    encodeByteArray[9] = (uchar)((0x0000ff00&this->getDLC())>>8);
    encodeByteArray.replace(10, 1014, this->body);

//    qDebug()<<"dataType:"<<(uchar)encodeByteArray[0] + ((uchar)encodeByteArray[1]<<8);


    return encodeByteArray;
}

//!
//! \brief igtDatagram::decode
//! \param byteArray
//!
void igtDatagram::decode(QByteArray byteArray){

    this->dataType = (unsigned int)((0x000000ff & byteArray[0]) + ((0x000000ff & byteArray[1])<<8));
    this->origineId = (unsigned int)(0x000000ff & byteArray[2]);
    this->targetId = (unsigned int)(0x000000ff & byteArray[3]);
    this->timeStampes = (unsigned int)((0x000000ff & byteArray[4]) + ((0x000000ff & byteArray[5])<<8) + ((0x000000ff & byteArray[6])<<16) + ((0x000000ff & byteArray[7])<<24));

    //qDebug()<<"valeur:"<<(unsigned char)byteArray[0]<<(unsigned char)byteArray[1]<<(unsigned char)byteArray[2]<<(unsigned char)byteArray[3]<<this->timeStampes;

    this->DLC = (int)((0x000000ff & byteArray[8]) + ((0x000000ff & byteArray[9])<<8));
    this->body = byteArray.mid(10, 1014);
}

//!
//! \brief igtDatagram::print
//!
void igtDatagram::print(){
    qDebug()<<"igtDatagram dataType: "<<this->dataType;
    qDebug()<<"igtDatagram origineId: "<<this->origineId;
    qDebug()<<"igtDatagram targetId: "<<this->targetId;
    qDebug()<<"igtDatagram timeStampes: "<<this->timeStampes;
    qDebug()<<"igtDatagram DLC: "<<this->DLC;
    qDebug()<<"igtDatagram MotorType: "<<(uchar)this->body[0];
    qDebug()<<"igtDatagram symbol: "<<(uchar)this->body[1];
    qDebug()<<"igtDatagram rpm: "<<(uchar)this->body[2] + (uchar)this->body[3];
}













