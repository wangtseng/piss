#ifndef PISSSERVER_H
#define PISSSERVER_H

#pragma comment(lib, "WS2_32")

#include<QtGlobal>
#include<QThread>
#include<WinSock2.h>
#include<Windows.h>
#include<WS2tcpip.h>
#include<iostream>
#include<stdio.h>
#include<assert.h>


//#include <QTcpServer>
#include <QVector>
#include <QMessageBox>

#include "pissDevices.h"
#include "pissReceptionTask.h"
#include "pissDatagrammeAnalyser.h"
#include "pissInputInformationDecoder.h"
#include "pissBaseDeDonnee.h"


/**
 * @brief The pissServer class
 */
class pissServer : public QThread{
    Q_OBJECT
public:

    bool init();
    void run();
    bool launchServer();
    bool stopServer();
    bool getConnectionState();


private:

    SOCKET socketServer;

    bool flag;
    bool serverStatus;

    pissDevices *devices;

    QVector <pissInputQueue*> *inputQueueManager;
    QVector <pissOutputQueue*> *outputQueueManager;
    pissDatagrammeAnalyser *datagrammeAnalyser;
    GlobalTime *globalTime;

    pissBaseDeDonnee* database;

signals:
    void incomingConnection(SOCKET sClient);

public:
    pissServer(QVector <pissInputQueue*> *inputQueueManager,
               QVector <pissOutputQueue*> *outputQueueManager,
               pissDevices* devices,
               pissDatagrammeAnalyser *datagrammeAnalyser,
               GlobalTime *globalTime,
               pissBaseDeDonnee* database);

    ~pissServer();
protected:
//    void incomingConnection(qintptr socket);
};

#endif // PISSSERVER_H
