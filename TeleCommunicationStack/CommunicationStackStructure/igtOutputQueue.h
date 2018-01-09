#ifndef IGTOUTPUTQUEUE_H
#define IGTOUTPUTQUEUE_H
#include <QList>
#include <QByteArray>
#include "igtDatagram.h"
#include "OmegaDataware.h"
#include <QMutex>
#include <QDebug>

class igtOutputQueue
{
private:
    QList<igtDatagram *> outputQueue;
    QMutex mutex;

public:
    igtDatagram *getLatestMessage();
    int getOutputQueueLength();
    void append(igtDatagram *sendMessage);
    void deleteLatestFrame();

 public:
    igtOutputQueue();
};

#endif // IGTOUTPUTQUEUE_H
