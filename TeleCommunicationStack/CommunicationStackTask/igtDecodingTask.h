#ifndef IGTDECODINGTASK_H
#define IGTDECODINGTASK_H

#include <QThread>
#include "igtInputQueueManager.h"
#include "igtAdvancementVelocityIs.h"
#include "pissBaseDeDonnee.h"
#include "igtDatagramAnalyser.h"


class igtDecodingTask : public QThread
{
    Q_OBJECT
public:
    void run();
    void stop();

private:
    pissBaseDeDonnee *dataware;
    igtInputQueueManager *inputQueueManager;
    igtDatagramAnalyser *datagramAnalyser;
    bool flag;

public:
    explicit igtDecodingTask(pissBaseDeDonnee *dataware, igtInputQueueManager *inputQueueManager, igtDatagramAnalyser *datagramAnalyser);

};

#endif // IGTDECODINGTASK_H
