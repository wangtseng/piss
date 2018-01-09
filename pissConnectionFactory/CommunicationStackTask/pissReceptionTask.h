#ifndef PISSHANDSHAKE_H
#define PISSHANDSHAKE_H

#pragma comment(lib, "WS2_32")

#include <QtGlobal>
#include <WinSock2.h>

#include <QThread>
#include <QDebug>
//#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include <QMutex>
#include "pissDatagrammeAnalyser.h"
#include "pissCDatagramme.h"


class pissReceptionTask : public QThread
{
    Q_OBJECT
public:
    pissReceptionTask(int id,
                    SOCKET SD,
                    pissDatagrammeAnalyser *datagrammeAnalyser,
                    QVector <pissInputQueue*> *inputQueueManager,
                    pissDevices *networkEnvironment);//socket descriptor
    ~pissReceptionTask();
    void run();

private:

    bool flag;
    int id;
    int clientCount;
    pissDatagrammeAnalyser *datagrammeAnalyser;
    QVector <pissInputQueue*> *inputQueueManager;
    pissDevices *networkEnvironment;
    SOCKET sd;
//    QTcpSocket *soc;
    qint16 blockSize;
    QString message;

    quint16 AIM;
    quint16 transType;
    quint16 DLC;
    QString currentTime;

    qint16 bytesReceived;
    qint16 totalBytes;
    QMutex mutexInputqueueManager;

public slots:
    //void lecture();
    //void disconnect();
};

#endif // PISSHANDSHAKE_H
