#ifndef IGTDATAGRAMANALYSER_H
#define IGTDATAGRAMANALYSER_H

#include <QByteArray>
#include "igtDatagram.h"
#include "igtMotorParameter.h"
#include "igtHelloMessage.h"
#include "igtHandShakeMessage.h"
#include "igtHandShakeCommitMessage.h"
#include "pissBaseDeDonnee.h"
#include "igtAdvancementVelocityIs.h"
#include "igtForceTorqueValues.h"
#include "igtGuidewireMovingDistance.h"
#include "igtContrastMediaPush.h"


typedef enum{
    HelloMsg = 0,
    HandShakeMsg,
    HandShakeCommitMsg,
    MotorMsg,
    VelocityIsMsg,
    CTImage,
    ForceFeedback,
    NDIPosition,
    GuidewireMovingDistance,
    ContrastMediaPush,
    CloseSessionMessage
}DatagrammeIdentifierCode;


class igtDatagramAnalyser : public QObject
{
    Q_OBJECT
private:
    pissBaseDeDonnee* dataware;
    int handshakeInstructionCount;
public:
    void analyse(igtDatagram *datagram);

    void transformIgtDatagramToMotorParameter(igtDatagram *datagram, igtMotorParameter *motorParameter);
    void transformMotorParameterToIgtDatagram(igtDatagram *datagram, igtMotorParameter *motorparameter);
    void transformInjectionCommandToIgtDatagram(igtDatagram *datagram,  igtContrastMediaPush * injectionCommand);
    void transformCloseSessionMessageToIgtDatagram(igtDatagram *datagram,  igtCloseSessionMessage * injectionCommand);

    void decodeHelloMsg(igtDatagram *datagram);
    void decodeHandShakeMsg(igtDatagram *datagram);
    void decodeHandShakeCommitMsg(igtDatagram *datagram);
    void decodeVelocityIsMsg(igtDatagram *datagram);
    void decodeForceTorqueValues(igtDatagram *datagram);
    void decodeNDIPosition(igtDatagram *datagram);
    void decodeGuidewireMovingDistance(igtDatagram *datagram);

signals:
    void newConnection(QString remoteIP);
    void connectionConfirm();
public:
    igtDatagramAnalyser(pissBaseDeDonnee* dataware);
};

#endif // IGTDATAGRAMANALYSER_H
