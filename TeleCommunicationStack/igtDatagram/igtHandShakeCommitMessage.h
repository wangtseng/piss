#ifndef HANDSHAKECOMMITMESSAGE_H
#define HANDSHAKECOMMITMESSAGE_H
#include <QString>
#include <QStringList>
#include "igtDatagram.h"

class igtHandShakeCommitMessage
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

    igtDatagram* transformHandShakeCommitMessageToIgtDatagram();
    void transformIgtDatagramToHandShakeCommitMessage(igtDatagram *datagram);
private:
    unsigned char addr[4];//!4字节
    int port;       //!2字节
public:
    igtHandShakeCommitMessage();
};

#endif // HANDSHAKECOMMITMESSAGE_H
