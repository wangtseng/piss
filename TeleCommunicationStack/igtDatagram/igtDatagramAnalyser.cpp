#include "igtDatagramAnalyser.h"
#include <QDebug>


/**
 * @brief igtDatagramAnalyser::igtDatagramAnalyser
 * @param dataware
 */
igtDatagramAnalyser::igtDatagramAnalyser(pissBaseDeDonnee* dataware)
{
    this->dataware = dataware;
    handshakeInstructionCount=0;
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::analyse
//! \param datagram
//!
void igtDatagramAnalyser::analyse(igtDatagram *datagram){
    switch(datagram->getDataType()){
        case HelloMsg:{
            decodeHelloMsg(datagram);
            break;
        }
        case HandShakeMsg:{
            if(handshakeInstructionCount == 0){
                decodeHandShakeMsg(datagram);
                handshakeInstructionCount ++;
            }
            break;
        }
        case HandShakeCommitMsg:{
            decodeHandShakeCommitMsg(datagram);
            break;
        }
        case MotorMsg:{
            break;
        }
        case VelocityIsMsg:{
            igtAdvancementTargetVelocity *advancementTargetVelocity = new igtAdvancementTargetVelocity();
            advancementTargetVelocity->transformIgtdatagramToAdvancementTargetVelocity(datagram);
            this->dataware->getPatientHandling()->appendAdvancementTargetVelocity(advancementTargetVelocity);
            break;
        }
        case CTImage:{
            break;
        }
        case ForceFeedback:{
            decodeForceTorqueValues(datagram);
            break;
        }
        case NDIPosition:{
            decodeNDIPosition(datagram);
            break;
        }
        case GuidewireMovingDistance:{
            decodeGuidewireMovingDistance(datagram);
            break;
        }
     }
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::transformIgtDatagramToMotorParameter
//! \param datagram
//! \param motorParameter
//!
void igtDatagramAnalyser::transformIgtDatagramToMotorParameter(igtDatagram *datagram, igtMotorParameter *motorParameter){
    if(datagram == NULL || motorParameter == NULL){
        return;
    }
    motorParameter->setMotorType((uchar)datagram->getIgtDatagramBody()[0]);
    motorParameter->setSymbol((uchar)datagram->getIgtDatagramBody()[1]);
    motorParameter->setRPM((uchar)datagram->getIgtDatagramBody()[2] + (uchar)datagram->getIgtDatagramBody()[3]);
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::transformCloseSessionMessageToIgtDatagram
//! \param datagram
//! \param injectionCommand
//!
void igtDatagramAnalyser::transformCloseSessionMessageToIgtDatagram(igtDatagram *datagram,  igtCloseSessionMessage * injectionCommand){
    if(datagram == NULL || injectionCommand == NULL){
        return;
    }

    QByteArray ret;
    datagram->setDataType(10);
    datagram->setOrigineId(1);
    datagram->setTargetId(1);
    datagram->setTimeStampes(12);
    datagram->setDLC(0);

    datagram->setIgtDatagramBody(ret);
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::transformInjectionCommandToIgtDatagram
//! \param datagram
//! \param injectionCommand
//!
void igtDatagramAnalyser::transformInjectionCommandToIgtDatagram(igtDatagram *datagram,  igtContrastMediaPush * injectionCommand){
    if(datagram == NULL || injectionCommand == NULL){
        return;
    }

    QByteArray ret;
    datagram->setDataType(9);
    datagram->setOrigineId(1);
    datagram->setTargetId(1);
    datagram->setTimeStampes(12);
    datagram->setDLC(12);
    ret[0] = (uchar)(int(injectionCommand->getVolume()*100)%256);
    ret[1] = (uchar)(int(injectionCommand->getVolume()*100)/256);
    ret[2] = (uchar)(int(injectionCommand->getSpeed()*100)%256);
    ret[3] = (uchar)(int(injectionCommand->getSpeed()*100)/256);

    datagram->setIgtDatagramBody(ret);
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::transformMotorParameterToIgtDatagram
//! \param datagram
//! \param motorparameter
//!
void igtDatagramAnalyser::transformMotorParameterToIgtDatagram(igtDatagram *datagram, igtMotorParameter *motorparameter){
    if(datagram == NULL || motorparameter == NULL){
        return;
    }

    QByteArray ret;
    datagram->setDataType(3);
    datagram->setOrigineId(1);
    datagram->setTargetId(1);
    datagram->setTimeStampes(12);
    datagram->setDLC(12);
    ret[0] = (uchar)(0x000000ff&motorparameter->getMotorType());
    ret[1] = (uchar)(0x000000ff&motorparameter->getSymbol());
    ret[2] = (uchar)(0x000000ff&motorparameter->getRPM());
    ret[3] = (uchar)((0x0000ff00&motorparameter->getRPM())>>8);
//    ret[4] = (uchar)(0x000000ff&motorparameter->getRound());
//    ret[5] = (uchar)((0x0000ff00&motorparameter->getRound())>>8);

    datagram->setIgtDatagramBody(ret);

    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeHelloMsg
//! \param datagram
//!
void igtDatagramAnalyser::decodeHelloMsg(igtDatagram *datagram){
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeHandShakeMsg
//! \param datagram
//!
void igtDatagramAnalyser::decodeHandShakeMsg(igtDatagram *datagram){
    igtHandShakeMessage *handShakeMessage = new igtHandShakeMessage();
    handShakeMessage->transformIgtDatagramToHandShakeMessage(datagram);
    QString RIP = handShakeMessage->getAddr();
    emit newConnection(RIP);
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeHandShakeCommitMsg
//! \param datagram
//!
void igtDatagramAnalyser::decodeHandShakeCommitMsg(igtDatagram *datagram){
    emit connectionConfirm();
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeVelocityIsMsg
//! \param datagram
//!
void igtDatagramAnalyser::decodeVelocityIsMsg(igtDatagram *datagram){
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeForceTorqueValues
//! \param datagram
//!
void igtDatagramAnalyser::decodeForceTorqueValues(igtDatagram *datagram){
    igtForceTorqueValues *forceTorqueValues = new igtForceTorqueValues();
    forceTorqueValues->transformIgtdatagramToForceFeedback(datagram);
    this->dataware->getPatientHandling()->appendForceTorqueValues(forceTorqueValues);
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeNDIPosition
//! \param datagram
//!
void igtDatagramAnalyser::decodeNDIPosition(igtDatagram *datagram){
    igtNDIPosition *NDIPosition = new igtNDIPosition();
    NDIPosition->transformIgtdatagramToNDIPosition(datagram);
    this->dataware->getPatientHandling()->appendNDIPosition(NDIPosition);
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeGuidewireMovingDistance
//! \param datagram
//!
void igtDatagramAnalyser::decodeGuidewireMovingDistance(igtDatagram *datagram){
    igtGuidewireMovingDistance *dist = new igtGuidewireMovingDistance();
    dist->transformIgtdatagramToDistance(datagram);
    this->dataware->getPatientHandling()->appendGuidewireMovingDistanceSequence(dist->getValue());
}
