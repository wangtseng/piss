#ifndef IGTSERVER_H
#define IGTSERVER_H

#include <QTcpServer>
#include <QMessageBox>
#include "igtReceptionTask.h"
#include "igtDatagramAnalyser.h"
#include "igtInputQueueManager.h"
#include <QNetworkInterface>

class igtServer: public QTcpServer
{
    Q_OBJECT
public:
    bool launchServer();
    bool stopServer();
    void stopServerThread();
    int getIncomingConnectionCount();
    QString ipDetect();
    void setMode(int mode);

private:
    int mode;
    int incomingConnectionCount;
    bool isConnected;
    igtInputQueueManager *inputQueueManager;
    QVector<igtReceptionTask*> receptionTaskManager;

protected:
    void incomingConnection(qintptr handle);

signals:
    void localIPDetect(QString locaIP);

public:
    igtServer(igtInputQueueManager *inputQueueManager);
    ~igtServer();
};

#endif // IGTSERVER_H
