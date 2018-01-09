#ifndef PISSINPUTINFORMATIONDECODER_H
#define PISSINPUTINFORMATIONDECODER_H

#include <QThread>
#include <QVector>
#include <QMutex>
#include "pissDatagrammeAnalyser.h"
#include "pissCDatagramme.h"

class pissInputInformationDecoder : public QThread
{
public:
    pissInputInformationDecoder(QVector <pissInputQueue*> *inputQueueManager,
                                pissDevices* NetworkEnvironment,
                                pissDatagrammeAnalyser *datagrammeAnalyser);
    ~pissInputInformationDecoder();
    void run();
    void stop();

private:
    pissDevices *networkEnvironment;
    QVector <pissInputQueue*> *inputQueueManager;
    pissDatagrammeAnalyser *datagrammeAnalyser;
    pissCDatagramme data;
    QMutex inputMutex;

    bool flag;
};

#endif // PISSINPUTINFORMATIONDECODER_H
