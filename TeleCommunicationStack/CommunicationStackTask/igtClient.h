#ifndef IGTCLIENT_H
#define IGTCLIENT_H
#include <QTcpSocket>
#include <QLineEdit>
#include <QHostAddress>
#include <QTimer>
#include <QThread>
#include <QNetworkInterface>
#include "igtHandShakeMessage.h"
#include "igtOutputQueueManager.h"
#include "igtHandShakeCommitMessage.h"


class igtClient:public QThread
{
    Q_OBJECT
private:
    int index;
    bool connectMotivateOrBack;
    bool flag;
    bool connectstate;
    QString targetIpLineEdit;
    QString targetPortLineEdit;
    QString localIP;
    QTcpSocket *connectTcpsocket;
    igtOutputQueueManager *outputQueueManager;
    QTimer *timer;
    int mode;

public:
    igtClient(int mode, int index, igtOutputQueueManager *outputQueueManager, QString targetIpLineEdit,QString targetPortLineEdit,bool MB);
    ~igtClient();

public:
    QString ipDetect();
    void sendHandShakeCommitMessage();
    void sendHandShakeMessage();
    bool getConnectionState();

signals:
    void connectionEstablished();

public slots:
    void wellConnected();
    void write();
    void sendHandShakeOrCommitMessage();
    void connectera();

};

#endif // IGTCLIENT_H
