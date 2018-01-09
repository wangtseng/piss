#ifndef PISSOUTPUTINFORMATIONENCODER_H
#define PISSOUTPUTINFORMATIONENCODER_H

#include <QThread>
#include <QVector>
#include <QMutex>
#include "pissDatagrammeAnalyser.h"
#include "pissCDatagramme.h"

class pissOutputInformationEncoder : public QThread
{

public:
    void run();
    void stop();

private:
    pissDevices *networkEnvironment;
    QVector <pissOutputQueue*> *outputQueueManager;
    pissDatagrammeAnalyser *datagrammeAnalyser;
    pissCDatagramme data;
    QMutex inputMutex;

    bool flag;

public:
    pissOutputInformationEncoder(QVector<pissOutputQueue *> *outputQueueManager,
                                 pissDevices *NetworkEnvironment,
                                 pissDatagrammeAnalyser *datagrammeAnalyser);
    ~pissOutputInformationEncoder();
};

#endif // PISSOUTPUTINFORMATIONENCODER_H
