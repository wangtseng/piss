#include "igtHandShakeCommitMessage.h"

igtHandShakeCommitMessage::igtHandShakeCommitMessage()
{

}

QString igtHandShakeCommitMessage::getAddr(){
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

void igtHandShakeCommitMessage::setAddr(QString address){
//    qDebug()<<address;
    QStringList addrs = address.split(".");
//    qDebug()<<addrs;
    addr[0] = (uchar)addrs[0].toInt();
    addr[1] = (uchar)addrs[1].toInt();
    addr[2] = (uchar)addrs[2].toInt();
    addr[3] = (uchar)addrs[3].toInt();
//    qDebug()<<addrs;
}

void igtHandShakeCommitMessage::setAddr(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree){
    addr[0] = addrZero;
    addr[1] = addrOne;
    addr[2] = addrTwo;
    addr[3] = addrThree;
}

int igtHandShakeCommitMessage::getPort(){
    int ret = this->port;
    return ret;
}

void igtHandShakeCommitMessage::setPort(int port){
    this->port = port;
}

void igtHandShakeCommitMessage::setPort(uchar portZero, uchar portOne){
    this->port = (int)(portZero + portOne*256);
}

igtDatagram* igtHandShakeCommitMessage::transformHandShakeCommitMessageToIgtDatagram(){

   igtDatagram *datagram = new igtDatagram();
   QByteArray array;
   array.resize(1024);

   array[0] = (uchar)(0x000000ff&2);
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

void igtHandShakeCommitMessage::transformIgtDatagramToHandShakeCommitMessage(igtDatagram *datagram){
    QByteArray ret;
    if(datagram == NULL){
        return;
    }
    ret = datagram->getIgtDatagramBody();

    this->setAddr((uchar)ret[0], (uchar)ret[1], (uchar)ret[2], (uchar)ret[3]);
    this->setPort((uchar)ret[4], (uchar)ret[5]);
}
