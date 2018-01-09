#ifndef IGTADDPARAMETERROTATETOOUTPUTQUEUETASK_H
#define IGTADDPARAMETERROTATETOOUTPUTQUEUETASK_H
#include <QThread>
#include "igtMotorParameter.h"
#include "igtDatagram.h"
#include "igtDatagramAnalyser.h"
#include "pissBaseDeDonnee.h"
#include "igtOutputQueueManager.h"

class igtAddParameterRotateToOutputqueueTask : public QThread
{
private:
    bool flag;
    pissBaseDeDonnee *dataware;
    igtDatagramAnalyser *datagramAnalyser;
    igtOutputQueueManager *outputQueueManager;

public:
    void run();
    void stop();

public:
    igtAddParameterRotateToOutputqueueTask(pissBaseDeDonnee *dataware, igtOutputQueueManager *outputQueueManager, igtDatagramAnalyser *datagramAnalyser);
};

#endif // IGTADDPARAMETERROTATETOOUTPUTQUEUETASK_H
