#ifndef PISSNETWORKENVIRONMENT_H
#define PISSNETWORKENVIRONMENT_H

#pragma comment(lib, "WS2_32")

#include <QtGlobal>
#include <WinSock2.h>
#include <QString>
#include <QHostAddress>
#include <QList>
#include <QNetworkInterface>
#include <QMutex>
#include "pissDevice.h"


/**
 * @brief The Devices class
 */
class pissDevices:public QObject
{
    Q_OBJECT

public:
    void append(QString ModuleName, int Port);
    void append(int ModuleNumber, int Port);
    void append(QString ModuleName, int Port, QString addr, int Socketrec, quint32 Clientlistenport, quint8 Code);
    void append(int ModuleNumber, int Port, QString addr, int Socketrec, quint32 Clientlistenport, quint8 Code);

    void setIpAddressById(int id, QString ip);
    void setSocketTransById(int id, SOCKET st);
    void setSocketReceptionById(int id, SOCKET sr);
    void setClientlistenportById(int id, quint32 Clientlistenport);
    void setCodeByModule(QString ModuleName, bool Code);

    QHostAddress getIpAddressByModule(QString ModuleName);
    QHostAddress getIpAddressByModule(int ModuleNumber);
    QString ipDetect();

    int getPortByModule(QString ModuleName);
    int getPortByModule(int ModuleNumber);

    int getSocketrecByModule(QString ModuleName);
    int getSockettransByModule(QString ModuleName);
    int getSockettransByCode(quint8 Code);
    quint32 getClientlistenportByModule(QString ModuleName);
    bool getCodeByModule(QString ModuleName);

    int addClient();
    int getClientNumber();

    pissDevice* getDeviceAt(int index);
    pissDevice* getLocalDevice();

    void setMyListenPort(quint32 port);
    int getMyListenPort();

private:
    int incommingDeviceCount;
    QMutex mutex;
    QList <pissDevice*> incomingDevices;
    pissDevice* localDevice;

signals:
    void update();

public:
    pissDevices();
};

#endif // PISSNETWORKENVIRONMENT_H
