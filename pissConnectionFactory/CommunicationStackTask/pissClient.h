#ifndef PISSCLIENT_H
#define PISSCLIENT_H

#include <QTcpSocket>
#include<QtGlobal>

#pragma comment(lib, "WS2_32")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
#include<WinSock2.h>
#include<Windows.h>
#include<WS2tcpip.h>

#include<iostream>
#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<QString>
#include<QStringList>


#include "pissTransmissionTask.h"
#include "pissHandShakeMessage.h"
#include "pissDevices.h"
#include "pissDevice.h"
#include "GlobalTime.h"
#include "pissHandShakeCommitMessage.h"


class pissClient : public QObject
{
    Q_OBJECT
private:
    SOCKET connectSocket;

    pissTransmissionTask *transmissionTask;
    QVector <pissOutputQueue*> *oq;
    pissOutputQueue* outputQueue;
    pissDevices* devices;
    bool motivateConnectionRequest;
    int id;
    GlobalTime *globalTime;
    QString addr;
    int port;
public:
    void init();
    SOCKET connectRequest();
    SOCKET connectBackRequest();

public slots:
    void startTransfer();
    void slotDisconnected();

public:
    pissClient(int id, QVector <pissOutputQueue*> *oq, pissDevices* devices, GlobalTime *globalTime, QString addr, int port);

};

#endif // IGTCLIENT_H

