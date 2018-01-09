#ifndef OUTPUTQUEUE_H
#define OUTPUTQUEUE_H

#include <QList>
#include <QMutex>
#include "pissCDatagramme.h"


class pissOutputQueue
{
public:
    pissOutputQueue();
    ~pissOutputQueue();

    void append(pissCDatagramme* datagramme);
    void clear();
    void deleteFrontElement();
    pissCDatagramme* fetchFirstDatagramme();
    int getLength();
private:
    QList <pissCDatagramme*> outputqueue;
    QMutex oqMutex;
    long cpt;
};

#endif // OUTPUTQUEUE_H
