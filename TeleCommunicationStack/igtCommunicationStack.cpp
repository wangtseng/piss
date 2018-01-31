#include "igtCommunicationStack.h"

/**
 * @brief igtCommunicationStack::igtCommunicationStack
 */
igtCommunicationStack::igtCommunicationStack(pissBaseDeDonnee* database){

    this->igtClientCount = 0;
    int mode = 192;

    this->database = database;
    this->inputQueueManager = new igtInputQueueManager();
    this->outputQueueManager = new igtOutputQueueManager();
    this->datagramAnalyser = new igtDatagramAnalyser(database);

    this->serv = new igtServer(inputQueueManager);
    this->serv->setMode(mode);
    this->connect(this->serv, SIGNAL(localIPDetect(QString)), this, SLOT(getSelfIp(QString)));

    this->encodingTask = new igtEncodingTask(this->database, this->outputQueueManager, this->datagramAnalyser);
    this->decodingTask = new igtDecodingTask(this->database, inputQueueManager, this->datagramAnalyser);

    this->connect(this->datagramAnalyser, SIGNAL(newConnection(QString)), this, SLOT(receptConnection(QString)));
    this->connect(this->datagramAnalyser, SIGNAL(connectionConfirm()), this, SLOT(connectionEstablish()));

}

//! --------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::getIncomingConnectionCount
//! \return
//!
int igtCommunicationStack::getIncomingConnectionCount(){
    return this->serv->getIncomingConnectionCount();
}

//! --------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::lauchServer
//! \return
//!
bool igtCommunicationStack::lauchServer(){
    bool ret;
    ret = serv->launchServer();
    this->decodingTask->start();
    this->encodingTask->start();
    return ret;
}

//! --------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::stopServer
//! \return
//!
bool igtCommunicationStack::stopServer(){   
    bool ret = false;
     ret = this->serv->stopServer();
//    this->server->stopServerThread();
//    this->stopDecodingTaskThread();
//    ret = true;
    return ret;
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::getSelfIp
//! \param addr
//!
void igtCommunicationStack::getSelfIp(QString addr){
    //qDebug()<<"igtCommunicationStack::getSelfIp"<<addr;
    //! emit....
    //!
    QString ret = addr;
    emit localIPDetect(ret);
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::stopDecodingTaskThread
//!
void igtCommunicationStack::stopDecodingTaskThread(){
    decodingTask->stop();
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::connectera
//! \param targetIpLineEdit
//! \param targetPortLineEdit
//! \return
//!
bool igtCommunicationStack::connectera(QString targetIpLineEdit,QString targetPortLineEdit, bool MB){
    bool ret = false;
    igtClient *client = new igtClient(this->mode, this->igtClientCount, outputQueueManager, targetIpLineEdit, targetPortLineEdit, MB);
    client->connectera();

    //ret = client->getConnectionState();
    //if(ret){
        igtOutputQueue *outputQueue = new igtOutputQueue();
        this->outputQueueManager->addIgtOutputQueue(outputQueue);
        //this->addHelloMessageTask = new igtAddHelloMessageTask(this->igtClientCount, outputQueueManager);
        //this->connect(client, SIGNAL(connectionEstablished()), this->addHelloMessageTask, SLOT(start()));

        this->igtClientCount ++;
        this->clientList.append(client);
        ret = true;
//    }
//    else{
//        delete client;
//    }


    return ret;
}

//! ----------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::receptConnection
//! \param remoteIP
//!
void igtCommunicationStack::receptConnection(QString remoteIP){
    QString ret = remoteIP;
    emit newConnection(ret);
}

//! -----------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::connectionEstablish
//!
void igtCommunicationStack::connectionEstablish(){
    this->clientList.at(0)->wellConnected();
    //this->addHelloMessageTask->start();
}
