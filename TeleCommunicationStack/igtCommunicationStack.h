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
#include "igtAddParameterRotateToOutputqueueTask.h"
#include "igtDatagramAnalyser.h"
#include "igtAddHelloMessageTask.h"


/**
 * @brief The igtCommunicationStack class
 */
class igtCommunicationStack : public QObject
{
    Q_OBJECT
private:
    pissBaseDeDonnee* database;                       //! system dataware

    igtServer *server;                           //! wait for new incoming connection
    QVector<igtClient*> clientList;

    igtInputQueueManager *inputQueueManager;     //! where to manage all the incoming message
    igtDecodingTask *decodingTask;               //! asynchrone task to decode incoming message periodic

    igtEncodingTask *encodingTask;

    igtAddParameterRotateToOutputqueueTask *addParameterRotateToOutputqueueTask;
    //igtAddHelloMessageTask *addHelloMessageTask;
    igtOutputQueueManager *outputQueueManager;   //! message center to be send according to the target device id in the message

    int igtClientCount;
    igtDatagramAnalyser *datagramAnalyser;

    int mode;

public:
    bool lauchServer();
    bool stopServer();
    int getIncomingConnectionCount();

    bool connectera(QString targetIpLineEdit,QString targetPortLineEdit, bool MB);

    void stopDecodingTaskThread();

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
