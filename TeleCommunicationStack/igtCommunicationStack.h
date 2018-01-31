#ifndef IGTCOMMUNICATIONSTACK_H
#define IGTCOMMUNICATIONSTACK_H

#include "igtServer.h"
#include "igtClient.h"
#include "igtInputQueueManager.h"
#include "igtDecodingTask.h"
#include "pissBaseDeDonnee.h"
#include "igtOutputQueue.h"
#include "igtOutputQueueManager.h"
#include <QTcpSocket>
#include <QObject>
#include <QVector>
#include "igtEncodingTask.h"
#include "igtDatagramAnalyser.h"
#include "igtAddHelloMessageTask.h"


/**
 * @brief The igtCommunicationStack class aim to build an asynchronouse communication mechanisme.
 *
 *  The software architecture include:
 *          server
 *
 *
 *
 */
class igtCommunicationStack : public QObject
{
    Q_OBJECT
private:
    pissBaseDeDonnee* database;                  //! system dataware

    igtServer *serv;                           //! local socket server wait for new incoming connection
    QVector<igtClient*> clientList;              //! client queue

    igtInputQueueManager *inputQueueManager;     //! where to manage all the incoming message
    igtOutputQueueManager *outputQueueManager;   //! message center to be send according to the target device id in the message

    igtDecodingTask *decodingTask;               //! asynchrone task to decode incoming message periodic
    igtEncodingTask *encodingTask;

    //igtAddHelloMessageTask *addHelloMessageTask;

    igtDatagramAnalyser *datagramAnalyser;       //! outil pour faire le decodage

    int igtClientCount;
    int mode;

public:
    bool lauchServer();
    bool stopServer();
    bool connectera(QString targetIpLineEdit,QString targetPortLineEdit, bool MB);
    void stopDecodingTaskThread();
    int getIncomingConnectionCount();

public slots:
    void getSelfIp(QString addr);
    void receptConnection(QString remoteIP);
    void connectionEstablish();

signals:
    void localIPDetect(QString addr);
    void newConnection(QString remoteIP);

public:
    igtCommunicationStack(pissBaseDeDonnee* database);
};

#endif // IGTCOMMUNICATIONSTACK_H
