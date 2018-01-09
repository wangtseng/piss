#ifndef MODULEIPCONFIG_H
#define MODULEIPCONFIG_H

#pragma comment(lib, "WS2_32")

#include <QtGlobal>
#include <WinSock2.h>
#include <QHostAddress>


typedef enum {
    PISS = 0,
    MISS,
    CCISS
}ModuleCode;


/**
 * @brief The Device class
 */
class pissDevice
{
public:
    pissDevice();

    QString getIpAddress();
    int getPortNumber();
    QString getName();
    int getSocketrec();
    int getSockettrans();
    quint32 getClientlistenport();
    bool getCode();
    int getDeviceNumber();

    void setIpAddress(QString Addr);
    void setPortNumber(int p);
    void setName(QString Name);
    void setSocketrec(SOCKET Socketrec);
    void setSockettrans(SOCKET Sockettrans);
    void setClientlistenport(quint32 Clientlistenport);
    void setCode(bool Code);
    void setId(int id);

private:
    int id;
    QString addr;
    int portNumber;
    QString name;
    SOCKET socketrec;
    SOCKET sockettrans;
    quint32 clientlistenport;
    bool code;
};

#endif // MODULEIPCONFIG_H
