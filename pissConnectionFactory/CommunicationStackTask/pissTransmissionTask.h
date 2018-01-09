#ifndef PISSTRANSMISSIONTASK_H
#define PISSTRANSMISSIONTASK_H

#pragma comment(lib, "WS2_32")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#include <QtGlobal>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <assert.h>
#include <QString>
#include <QStringList>


#include <QThread>
#include <QObject>
//#include <QTcpSocket>
#include <QTimer>
#include "GlobalTime.h"
#include "pissDevices.h"
#include "pissOutputQueue.h"


class pissTransmissionTask : public QThread
{
    Q_OBJECT

public:
    void launch();
    void run();
    void stop();

private:
    SOCKET transmissionSocket;
    QVector <pissOutputQueue*> *oq;
    pissDevices *devices;
    QTimer *transmissionTimer;
    int frameCounter;
    QMutex outputMutex;
    int id;
    bool flag;
public:
    pissTransmissionTask(int id, QVector <pissOutputQueue*> *oq, pissDevices* devices, SOCKET transmissionSocket);
    ~pissTransmissionTask();

public slots:
//    void transfer();
};

#endif // PISSTRANSMISSIONTASK_H
