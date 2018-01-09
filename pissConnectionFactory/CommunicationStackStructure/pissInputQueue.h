#ifndef INPUTQUEUE_H
#define INPUTQUEUE_H

#include<QVector>
#include"pissCDatagramme.h"

class pissInputQueue
{
public:
    pissInputQueue();
    ~pissInputQueue();

    void append(pissCDatagramme* datagramme);
    void clear();
    void deleteFirstDatagramme();
    pissCDatagramme* fetchFirstDatagramme();
    int getLength();

private:
    QVector <pissCDatagramme*> inputqueue;
};

#endif // INPUTQUEUE_H
