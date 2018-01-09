#ifndef CDATAGRAMME_H
#define CDATAGRAMME_H

#include "GlobalTime.h"

#include <string.h>
#include <cmath>
#include <QByteArray>
#include <qDebug>

class pissHandShakeMessage;
class pissHandShakeCommitMessage;
class pissHelloMessage;

class pissCDatagramme
{
public:

    pissCDatagramme();
    ~pissCDatagramme();

    void toCDatagram(pissHandShakeMessage *handshakeMessage);
    void toCDatagram(pissHandShakeCommitMessage *handshakeCommitMessage);
    void toCDatagram(pissHelloMessage *helloMessage);

    void setValue(char* v);
    const char* getValue();


    int getDataType();
    int getOrigineId();
    int getTargetId();
    int getTimeStampes();
    int getDLC();
    QString getAddr();
    int getPort();

    void print();
private:
    int totalLength;
    char* value;
};

#endif // CDATAGRAMME_H
