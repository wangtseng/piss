#include "pissHandShakeMessage.h"
#include "pissHandShakeCommitMessage.h"
#include "pissHelloMessage.h"

#include "pissCDatagramme.h"


/**
 * @brief CDatagramme::CDatagramme
 * @param value
 */
pissCDatagramme::pissCDatagramme(){

    this->value = new char[MAX_PACKET_SIZE+1];
    memset(this->value, '\0', MAX_PACKET_SIZE+1);

}

//!-------------------------------------------------------------
//!
//! \brief CDatagramme::~CDatagramme
//!
pissCDatagramme::~pissCDatagramme(){

}


void pissCDatagramme::print(){
    qDebug()<<"CDatagramme print:";
    qDebug()<<"datatype:"<<this->getDataType();
    qDebug()<<"origineId:"<<this->getOrigineId();
    qDebug()<<"getTargetId:"<<getTargetId();
    qDebug()<<"getDLC:"<<getDLC();
    qDebug()<<"getTimeStampes:"<<getTimeStampes();
    qDebug()<<"getAddr:"<<getAddr();
    qDebug()<<"getPort:"<<getPort();

}


//!-------------------------------------------------------------
//!
//! \brief toCDatagram
//! \param handshakeMessage
//!
void pissCDatagramme::toCDatagram(pissHandShakeMessage *handshakeMessage){

    QByteArray encodeByteArray;
       encodeByteArray.resize(MAX_PACKET_SIZE+1);

       encodeByteArray[0] = (uchar)(0x000000ff&handshakeMessage->getDataType());
       encodeByteArray[1] = (uchar)((0x0000ff00&handshakeMessage->getDataType())>>8);

       encodeByteArray[2] = (uchar)((0x0000000ff&handshakeMessage->getOrigineId()));

       encodeByteArray[3] = (uchar)((0x000000ff&handshakeMessage->getTargetId()));

       encodeByteArray[4] = (uchar)((0x000000ff&handshakeMessage->getTimeStampes()));
       encodeByteArray[5] = (uchar)((0x0000ff00&handshakeMessage->getTimeStampes())>>8);
       encodeByteArray[6] = (uchar)((0x00ff0000&handshakeMessage->getTimeStampes())>>16);
       encodeByteArray[7] = (uchar)((0xff000000&handshakeMessage->getTimeStampes())>>24);


       encodeByteArray[8] = (uchar)((0x000000ff&handshakeMessage->getDLC()));
       encodeByteArray[9] = (uchar)((0x0000ff00&handshakeMessage->getDLC())>>8);

       int addr[4];
       QStringList addrs = handshakeMessage->getAddr().split(".");
       QString temp1 = addrs[0];
       addr[0] = temp1.toInt();

       QString temp2 = addrs[1];
       addr[1] = temp2.toInt();

       QString temp3 = addrs[2];
       addr[2] = temp3.toInt();

       QString temp4 = addrs[3];
       addr[3] = temp4.toInt();

       qDebug()<<"addr[3]"<<addr[3];
       encodeByteArray[10] = (uchar)((0x000000ff&addr[0]));
       encodeByteArray[11] = (uchar)((0x000000ff&addr[1]));
       encodeByteArray[12] = (uchar)((0x000000ff&addr[2]));
       encodeByteArray[13] = (uchar)((0x000000ff&addr[3]));

       encodeByteArray[14] = (uchar)((0x000000ff&handshakeMessage->getPort()));
       encodeByteArray[15] = (uchar)((0x0000ff00&handshakeMessage->getPort())>>8);


       char *ch;
       ch = encodeByteArray.data();

       memcpy(this->value, ch, MAX_PACKET_SIZE+1);
}


//!-------------------------------------------------------------
//!
//! \brief toCDatagram
//! \param handshakeCommitMessage
//!
void pissCDatagramme::toCDatagram(pissHandShakeCommitMessage *handshakeCommitMessage){
    QByteArray encodeByteArray;
       encodeByteArray.resize(MAX_PACKET_SIZE+1);

       encodeByteArray[0] = (uchar)(0x000000ff&handshakeCommitMessage->getDataType());
       encodeByteArray[1] = (uchar)((0x0000ff00&handshakeCommitMessage->getDataType())>>8);

       encodeByteArray[2] = (uchar)((0x0000000ff&handshakeCommitMessage->getOrigineId()));

       encodeByteArray[3] = (uchar)((0x000000ff&handshakeCommitMessage->getTargetId()));

       encodeByteArray[4] = (uchar)((0x000000ff&handshakeCommitMessage->getTimeStampes()));
       encodeByteArray[5] = (uchar)((0x0000ff00&handshakeCommitMessage->getTimeStampes())>>8);
       encodeByteArray[6] = (uchar)((0x00ff0000&handshakeCommitMessage->getTimeStampes())>>16);
       encodeByteArray[7] = (uchar)((0xff000000&handshakeCommitMessage->getTimeStampes())>>24);


       encodeByteArray[8] = (uchar)((0x000000ff&handshakeCommitMessage->getDLC()));
       encodeByteArray[9] = (uchar)((0x0000ff00&handshakeCommitMessage->getDLC())>>8);

       int addr[4];
       QStringList addrs = handshakeCommitMessage->getAddr().split(".");
       QString temp1 = addrs[0];
       addr[0] = temp1.toInt();

       QString temp2 = addrs[1];
       addr[1] = temp2.toInt();

       QString temp3 = addrs[2];
       addr[2] = temp3.toInt();

       QString temp4 = addrs[3];
       addr[3] = temp4.toInt();

       qDebug()<<"addr[3]"<<addr[3];
       encodeByteArray[10] = (uchar)((0x000000ff&addr[0]));
       encodeByteArray[11] = (uchar)((0x000000ff&addr[1]));
       encodeByteArray[12] = (uchar)((0x000000ff&addr[2]));
       encodeByteArray[13] = (uchar)((0x000000ff&addr[3]));

       encodeByteArray[14] = (uchar)((0x000000ff&handshakeCommitMessage->getPort()));
       encodeByteArray[15] = (uchar)((0x0000ff00&handshakeCommitMessage->getPort())>>8);


       char *ch;
       ch = encodeByteArray.data();

       memcpy(this->value, ch, MAX_PACKET_SIZE+1);
    return;
}


//!-------------------------------------------------------------
//!
//! \brief toCDatagram
//! \param helloMessage
//!
void pissCDatagramme::toCDatagram(pissHelloMessage *helloMessage){
    return;
}


//!--------------------------------------------------------------
//!
//! \brief CDatagramme::getDataType
//! \return
//!
int pissCDatagramme::getDataType(){
    int ret;
    ret = (uchar)(this->value[0])+((uchar)(this->value[1])<<8);
    return ret;
}


//!--------------------------------------------------------------
//!
//! \brief CDatagramme::getOrigineId
//! \return
//!
int pissCDatagramme::getOrigineId(){
    int ret;
    ret =(uchar) this->value[2];
    return ret;
}


//!---------------------------------------------------------------
//!
//! \brief CDatagramme::getTargetId
//! \return
//!
int pissCDatagramme::getTargetId(){
    int ret;
    ret = (uchar)(this->value[3]);
    return ret;
}


//!---------------------------------------------------------------
//!
//! \brief CDatagramme::getTimeStampes
//! \return
//!
int pissCDatagramme::getTimeStampes(){
    int ret;
    ret = (uchar)(this->value[4])+((uchar)(this->value[5])<<8)+(((uchar)(this->value[6]))<<16)+(((uchar)(this->value[7]))<<24);
    return ret;
}


//!---------------------------------------------------------------
//!
//! \brief CDatagramme::getDLC
//! \return
//!
int pissCDatagramme::getDLC(){
    int ret;
    ret = (uchar)(this->value[8])+(((uchar)(this->value[9]))<<8);
    return ret;
}


//!---------------------------------------------------------------
//!
//! \brief CDatagramme::getAddr
//! \return
//!
QString pissCDatagramme::getAddr(){
//    QString addrs;
//    addrs.append(this->value[10]);
//    addrs.append(".");
//    addrs.append(this->value[11]);
//    addrs.append(".");
//    addrs.append(this->value[12]);
//    addrs.append(".");
//    addrs.append(this->value[13]);
//    return addrs;

    int addrInt[4];
    for(int i = 0; i < 4; i++){
        addrInt[i] = (uchar)this->value[i+10];
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


//!----------------------------------------------------------------
//!
//! \brief CDatagramme::getPort
//! \return
//!
int pissCDatagramme::getPort(){
    int ret;
    ret = (uchar)(this->value[14])+((uchar)(this->value[15])<<8);
    return ret;
}

//!-------------------------------------------------------------
//!
//! \brief CDatagramme::setValue
//! \param value
//!
void pissCDatagramme::setValue(char* v){
    //! qDebug()<<v;
    memcpy(this->value, v, MAX_PACKET_SIZE+1);
    //! this->value = v;
    //! this->totalLength = this->value.length();
}

//!-------------------------------------------------------------
//!
//! \brief CDatagramme::getValue
//! \return
//!
const char* pissCDatagramme::getValue(){
    return this->value;
}
