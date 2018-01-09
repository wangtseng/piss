#ifndef IGTENCODINGTASK_H
#define IGTENCODINGTASK_H

#include <QThread>
#include "igtMotorParameter.h"
#include "igtDatagram.h"
#include "igtDatagramAnalyser.h"
#include "pissBaseDeDonnee.h"
#include "igtOutputQueueManager.h"
#include "Patient.h"

class igtEncodingTask : public QThread
{
    Q_OBJECT

private:
    bool flag;
    pissBaseDeDonnee *dataware;
    igtDatagramAnalyser *datagramAnalyser;
    igtOutputQueueManager *outputQueueManager;

public:
    void run();
    void stop();
public:
    igtEncodingTask(pissBaseDeDonnee *dataware, igtOutputQueueManager *outputQueueManager, igtDatagramAnalyser *datagramAnalyser);
};

#endif // IGTENCODINGTASK_H
