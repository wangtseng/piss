#ifndef IGTOUTPUTQUEUEMANAGER_H
#define IGTOUTPUTQUEUEMANAGER_H

#include <QObject>
#include "igtOutputQueue.h"
#include <QVector>
#include "igtDatagram.h"
#include <QMutex>

class igtOutputQueueManager : public QObject
{
    Q_OBJECT
public:
    explicit igtOutputQueueManager(QObject *parent = 0);
    void addIgtOutputQueue(igtOutputQueue* outPutQueue);

    void setMessageToSendByTargetId(int ret, igtDatagram *sendMessage);
    igtDatagram *getLatestMessageByOutputQueueIndex(int index);
    void deleteLatestMessageByOutputQueueIndex(int index);
    int getLatestMessageLengthByOutputQueueIndex(int index);
    int getOutputQueueManagerLength();
signals:

public slots:
private:
    QMutex *mutex;
    QVector<igtOutputQueue*> outputQueueManager;
    int outputQueueManagerLength;
};

#endif // IGTOUTPUTQUEUEMANAGER_H
