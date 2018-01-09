#ifndef PISSCOMMUNICATIONSTACK_H
#define PISSCOMMUNICATIONSTACK_H

#pragma comment(lib, "WS2_32")

#include<WinSock2.h>

#include "pissInputInformationDecoder.h"
#include "pissOutputInformationEncoder.h"
#include "pissDevices.h"
#include "pissDatagrammeAnalyser.h"
#include "pissInputQueue.h"
#include "pissOutputQueue.h"
#include "pissServer.h"
#include "GlobalTime.h"
#include "pissClient.h"
#include "pissBaseDeDonnee.h"
#include "pissReceptionTask.h"
#include "pissTransmissionTask.h"

/**
 * @brief The pissCommunicationStack class
 */
class pissCommunicationStack:public QObject
{
    Q_OBJECT

public:
    void initComponents();
    void initRealTimeTasks();
    void setConnections();
    void setReceptionTask();

    bool launchServer();
    bool connectBack(bool flag, QString addr, int port);
    bool closeServer();
    void clearBuffer();
    void setDatabase(pissBaseDeDonnee* database);

    pissDevice* getLocalDevice();

private:
    pissDevices *devices;
    QVector <pissInputQueue*> inputQueueManager;
    QVector <pissOutputQueue*> outputQueueManager;
    pissDatagrammeAnalyser *datagrammeAnalyser;
    pissInputInformationDecoder *informationDecodeTask;
    pissOutputInformationEncoder *outputInformationEncoder;
    pissServer *server;
    GlobalTime *globalTime;
    pissBaseDeDonnee* database;

public slots:
    void connectBackRequest(QString ip, int port);
    void newReceptionTask(SOCKET sClient);

signals:
    void notifyPort(int port);

public:
    pissCommunicationStack(GlobalTime *globalTime, pissBaseDeDonnee* database);

};

#endif // PISSCOMMUNICATIONSTACK_H
