#ifndef HANDSHAKECOMMITMESSAGE_H
#define HANDSHAKECOMMITMESSAGE_H

#include "pissCDatagramme.h"

/**
 * @brief The HandShakeCommitMessage class
 */
class pissHandShakeCommitMessage{

public:

    void decodeDatagram(pissCDatagramme *datagram);


    int getDataType();
    void setDataType(int dataType);

    int getOrigineId();
    void setOrigineId(int origineId);

    int getTargetId();
    void setTargetId(int targetId);

    int getTimeStampes();
    void setTimeStampes(int timeStampes);

    int getDLC();
    void setDLC(int DLC);

    QString getAddr();
    void setAddr(QString address);

    int getPort();
    void setPort(int port);

    void print();
private:

    //! datagramme head
    int dataType;                           // 2 Bytes
    int origineId;                          // 1 Byte
    int targetId;                           // 1 Byte
    int timeStampes;                        // 4 Bytes
    int DLC;                                // 2 Bytes

    unsigned char addr[4];                  // 4 Bytes
    int port;                               // 2 Bytes


public:
    pissHandShakeCommitMessage();
    ~pissHandShakeCommitMessage();

};

#endif // HANDSHAKECOMMITMESSAGE_H
