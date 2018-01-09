#ifndef DATAGRAMMEANALYSER_H
#define DATAGRAMMEANALYSER_H

#include <QHostAddress>
#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QByteArray>
#include <QPair>
#include <QList>
#include "pissInputQueue.h"
#include "pissOutputQueue.h"
#include "pissCDatagramme.h"
#include "pissDevices.h"
#include "pissTransmissionTask.h"
#include "GlobalTime.h"
#include "pissClient.h"
#include "pissBaseDeDonnee.h"
#include "pissHandShakeMessage.h"
#include "pissHandShakeCommitMessage.h"

//! Codes des erreurs remontees par les fonctions
typedef enum {
    pissHelloMessage1 = 0,
    HandShakeMsg1 = 1,
    HandShakeCommitMsg1 = 2,
    CTImage1
}DatagrammeIdentifierCode1;

/**
 * @brief The DatagrammeAnalyser class
 *
 */
class pissDatagrammeAnalyser:public QObject
{
    Q_OBJECT
public:
    pissDatagrammeAnalyser(QVector <pissOutputQueue*> *oq, QVector <pissInputQueue*> *iq, pissDevices *environment, GlobalTime *globalTime, pissBaseDeDonnee* database);
    ~pissDatagrammeAnalyser();

    void encodage(QString modulename);
    void decodage(int id, pissCDatagramme *datagramme);

    void decodeHelloMessage(int id, pissCDatagramme *datagramme);
    void decodeHandShakeMessage(int id, pissCDatagramme *datagramme);
    void decodeHandShakeCommitMessage(int id, pissCDatagramme *datagramme);
    void decodeCTImage(pissCDatagramme *datagramme);
    void setConnectBackRequestWaitingPair(int waitingId, SOCKET waitingSocket);
    //int getDatagrammeLength();

signals:
    void handshakeMessageReactProcess(QString ip, int port);

private:
    DatagrammeIdentifierCode1 identifierCode;

    QVector <pissInputQueue*> *iq;
    QVector <pissOutputQueue*> *oq;

    pissDevices *devices;
    GlobalTime *globalTime;

    pissBaseDeDonnee* database;

    QList<QPair<int, SOCKET> > waitingList;
};

#endif // DATAGRAMMEANALYSER_H
