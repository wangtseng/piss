#ifndef IGTADDHELLOMESSAGETASK_H
#define IGTADDHELLOMESSAGETASK_H

#include <QThread>
#include "igtOutputQueueManager.h"
#include "igtHelloMessage.h"

class igtAddHelloMessageTask : public QThread
{
    Q_OBJECT
private:
    bool flag;
    int index;
    igtOutputQueueManager *outputQueueManager;
public:
    void run();
public:
    igtAddHelloMessageTask(int index, igtOutputQueueManager *outputQueueManager);
};

#endif // IGTADDHELLOMESSAGETASK_H
