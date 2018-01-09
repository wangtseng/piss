#include "igtHandShakeMessage.h"

igtHandShakeMessage::igtHandShakeMessage()
{
}

QString igtHandShakeMessage::getAddr(){
    QString addrs ;
    addrs.append(QString::number((int)addr[0]));
    addrs.append(".");
    addrs.append(QString::number((int)addr[1]));
    addrs.append(".");
    addrs.append(QString::number((int)addr[2]));
    addrs.append(".");
    addrs.append(QString::number((int)addr[3]));
    return addrs;
}

void igtHandShakeMessage::setAddr(QString address){
//    qDebug()<<address;
    QStringList addrs = address.split(".");
//    qDebug()<<addrs;
    addr[0] = (uchar)addrs[0].toInt();
    addr[1] = (uchar)addrs[1].toInt();
    addr[2] = (uchar)addrs[2].toInt();
    addr[3] = (uchar)addrs[3].toInt();
//    qDebug()<<addrs;
}

void igtHandShakeMessage::setAddr(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree){
    addr[0] = addrZero;
    addr[1] = addrOne;
    addr[2] = addrTwo;
    addr[3] = addrThree;
}

int igtHandShakeMessage::getPort(){
    int ret = this->port;
    return ret;
}

void igtHandShakeMessage::setPort(int port){
    this->port = port;
}

void igtHandShakeMessage::setPort(uchar portZero, uchar portOne){
    this->port = portZero + portOne*256;
}

//QByteArray HandShakeMessage::decode(){}

//QByteArray HandShakeMessage::encode(){

//   QByteArray encodeByteArray;
//   encodeByteArray.resize(1024);

//   encodeByteArray[0] = (uchar)(0x000000ff&this->getDataType());
//   encodeByteArray[1] = (uchar)((0x0000ff00&this->getDataType())>>8);

//   encodeByteArray[2] = (uchar)((0x0000000ff&this->getOrigineId()));

//   encodeByteArray[3] = (uchar)((0x000000ff&this->getTargetId()));

//   encodeByteArray[4] = (uchar)((0x000000ff&this->getTimeStampes()));
//   encodeByteArray[5] = (uchar)((0x0000ff00&this->getTimeStampes())>>8);
//   encodeByteArray[6] = (uchar)((0x00ff0000&this->getTimeStampes())>>16);
//   encodeByteArray[7] = (uchar)((0xff000000&this->getTimeStampes())>>24);

//   encodeByteArray[8] = (uchar)((0x000000ff&this->getDLC()));
//   encodeByteArray[9] = (uchar)((0x0000ff00&this->getDLC())>>8);

//   encodeByteArray[10] = (uchar)((0x000000ff&this->addr[0]));
//   encodeByteArray[11] = (uchar)((0x000000ff&this->addr[1]));
//   encodeByteArray[12] = (uchar)((0x000000ff&this->addr[2]));
//   encodeByteArray[13] = (uchar)((0x000000ff&this->addr[3]));

//   encodeByteArray[14] = (uchar)((0x000000ff&this->getPort()));
//   encodeByteArray[15] = (uchar)((0x0000ff00&this->getPort())>>8);


//   return encodeByteArray;
//}

void igtHandShakeMessage::transformIgtDatagramToHandShakeMessage(igtDatagram *datagram){
    QByteArray ret;
    if(datagram == NULL){
        return;
    }
    ret = datagram->getIgtDatagramBody();

    this->setAddr((uchar)ret[0], (uchar)ret[1], (uchar)ret[2], (uchar)ret[3]);
    this->setPort((uchar)ret[4], (uchar)ret[5]);
}

igtDatagram *igtHandShakeMessage::transformHandShakeMessageToIgtDatagram(){
    igtDatagram *datagram = new igtDatagram();
    QByteArray array;
    array.resize(1024);

    array[0] = (uchar)(0x000000ff&1);
    array[1] = (uchar)(0x000000ff&0);

    array[2] = (uchar)((0x0000000ff&1));

    array[3] = (uchar)((0x000000ff&1));

    array[4] = (uchar)(0x000000ff&15);
    array[5] = (uchar)(0x000000ff&00);
    array[6] = (uchar)(0x000000ff&00);
    array[7] = (uchar)(0xff000000&00);

    array[8] = (uchar)(0x000000ff&12);
    array[9] = (uchar)(0x000000ff&00);

    array[10] = (uchar)((0x000000ff&this->addr[0]));
    array[11] = (uchar)((0x000000ff&this->addr[1]));
    array[12] = (uchar)((0x000000ff&this->addr[2]));
    array[13] = (uchar)((0x000000ff&this->addr[3]));

    array[14] = (uchar)((0x000000ff&this->getPort()));
    array[15] = (uchar)((0x0000ff00&this->getPort())>>8);

    datagram->decode(array);
    return datagram;
}


