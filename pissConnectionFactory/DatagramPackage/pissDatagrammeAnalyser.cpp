#include "pissDatagrammeAnalyser.h"


pissDatagrammeAnalyser::pissDatagrammeAnalyser(QVector <pissOutputQueue*> *oq,
                                       QVector <pissInputQueue*> *iq,
                                       pissDevices* environment,
                                       GlobalTime *globalTime,
                                       pissBaseDeDonnee* database)
{
    this->iq = iq;
    this->oq = oq;
    //qDebug()<<"DA"<<oq;
    this->devices = environment;
    this->globalTime = globalTime;
    this->database = database;
}

pissDatagrammeAnalyser::~pissDatagrammeAnalyser()
{

}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::encodage
//! \param modulename
//!
void pissDatagrammeAnalyser::encodage(QString modulename)
{

}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::decodage
//! \param id
//! \param datagramme
//!
void pissDatagrammeAnalyser::decodage(int id, pissCDatagramme *datagramme){

    switch (datagramme->getDataType()){
        case pissHelloMessage1: {
            qDebug()<<"HelloMessage";
            decodeHelloMessage(id, datagramme);
            break;
        }
        case HandShakeMsg1:{
            qDebug()<<"HandShakeMessage";
            decodeHandShakeMessage(id, datagramme);
            break;
        }
        case HandShakeCommitMsg1:{
            qDebug()<<"HandShakeCommitMsg";
            decodeHandShakeCommitMessage(id, datagramme);
            break;
        }
        case CTImage1:{
            qDebug()<<"CTImage";
            decodeCTImage(datagramme);
            break;
        }
    }
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::decodeHelloMessage
//! \param id
//! \param datagramme
//!
void pissDatagrammeAnalyser::decodeHelloMessage(int id, pissCDatagramme *datagramme){
//    datagramme->setTimestamp(globalTime->currentTime());
    oq->at(id)->append(datagramme);
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::decodeHandShakeMessage
//! \param id
//! \param datagramme
//!
void pissDatagrammeAnalyser::decodeHandShakeMessage(int id, pissCDatagramme *datagramme){

    pissHandShakeMessage *msg = new pissHandShakeMessage();
    msg->decodeDatagram(datagramme);

    this->devices->setClientlistenportById(id, msg->getPort());

    emit handshakeMessageReactProcess(msg->getAddr(), msg->getPort());

}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::decodeHandShakeCommitMessage
//! \param id
//! \param datagramme
//!
void pissDatagrammeAnalyser::decodeHandShakeCommitMessage(int id, pissCDatagramme *datagramme){
    pissHandShakeCommitMessage *msg = new pissHandShakeCommitMessage();
    msg->decodeDatagram(datagramme);

    //devices->setSocketTransById(id, waitingList.at(id).second);
}

//! ----------------------------------------------------------------------------------------
//!
//! \brief DatagrammeAnalyser::decodeCTImage
//! \param datagramme
//!
void pissDatagrammeAnalyser::decodeCTImage(pissCDatagramme *datagramme){

}

void pissDatagrammeAnalyser::setConnectBackRequestWaitingPair(int waitingId, SOCKET waitingSocket){
    QPair<int, SOCKET> pair;
    pair.first = waitingId;
    pair.second = waitingSocket;
    waitingList.append(pair);
}
