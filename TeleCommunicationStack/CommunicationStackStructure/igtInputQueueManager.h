#ifndef IGTINPUTQUEUEMANAGER_H
#define IGTINPUTQUEUEMANAGER_H

#include <QVector>
#include <QObject>
#include <QTimer>
#include <QMutex>
#include "igtInputQueue.h"

class igtInputQueueManager:public QObject
{
    Q_OBJECT

public:
    void addIgtInputQueue(igtInputQueue* inputQueue);
    int getLength();

    int getDataArrayCountFromInputQueue(int i);
    igtDatagram *getDataArrayFromInputQueue(int i);
    void removeDataArrayFromInputQueue(int i);


public slots:
    void print();

private:
    QVector<igtInputQueue*>igtInputDataware;
    QTimer *printTimer;
    QMutex mutex;


public:
    igtInputQueueManager();
};

#endif // IGTINPUTQUEUEMANAGER_H
