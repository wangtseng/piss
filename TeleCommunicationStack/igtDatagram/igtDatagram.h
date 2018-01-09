#ifndef IGTDATAGRAM_H
#define IGTDATAGRAM_H
#include <QByteArray>

class igtDatagram
{
private:
    //! datagram head
    unsigned int dataType;   // 2 bytes
    unsigned int origineId;  // 1 byte
    unsigned int targetId;   // 1 byte
    unsigned int timeStampes;// 4 bytes
    unsigned int DLC;        // 2 byte;

    //! data body
    QByteArray body;

public:
    unsigned int getDataType();
    void setDataType(unsigned int dataType);

    unsigned int getTargetId();
    void setTargetId(unsigned int targetId);

    unsigned int getOrigineId();
    void setOrigineId(unsigned int origineId);

    unsigned int getTimeStampes();
    void setTimeStampes(unsigned int timeStampes);

    unsigned int getDLC();
    void setDLC(unsigned int DLC);

    QByteArray getIgtDatagramBody();
    void setIgtDatagramBody(QByteArray igtDatagramBody);

    void decode(QByteArray byteArray);
    QByteArray encode();
    void print();

public:
    igtDatagram();
};

#endif // IGTDATAGRAM_H
