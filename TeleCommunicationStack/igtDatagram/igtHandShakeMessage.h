#ifndef HANDSHAKEMESSAGE_H
#define HANDSHAKEMESSAGE_H

#include "igtDatagram.h"
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QDebug>

class igtHandShakeMessage
{
public:
    QString getAddr();
    void  setAddr(QString address);
    void setAddr(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree);
    int   getPort();
    void  setPort(int port);
    void setPort(uchar portZero, uchar portOne);
    // QByteArray decode();
    QByteArray encode();

    void transformIgtDatagramToHandShakeMessage(igtDatagram *datagram);
    igtDatagram *transformHandShakeMessageToIgtDatagram();
    //void printSelf();

private:
    unsigned char addr[4];//!4字节
    int port;       //!2字节

public:
    igtHandShakeMessage();
};

#endif // HANDSHAKEMESSAGE_H
