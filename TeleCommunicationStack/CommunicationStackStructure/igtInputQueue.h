#ifndef IGTINPUTQUEUE_H
#define IGTINPUTQUEUE_H

#include <QList>
#include <QByteArray>
#include <QDebug>
#include <QMutex>
#include <igtDatagram.h>


class igtInputQueue
{
public:
    void append(igtDatagram *datagram);
    void print();
    igtDatagram* getLatestArray();
    void deleteLatestArray();
    int getLength();

private:
    QMutex mutex;
    QList<igtDatagram *> inputQueue;

public:
    igtInputQueue();
};

#endif // IGTINPUTQUEUE_H
