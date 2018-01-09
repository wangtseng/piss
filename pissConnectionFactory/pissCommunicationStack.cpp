#include "pissCommunicationStack.h"


/**
 * @brief pissCommunicationStack::pissCommunicationStack
 *
 * The communication stack include four components and three permanent real time task. each time when an incoming device connect local device via protocole tcp/ip,
 *
 * a pair of task(reception&transmission) wil be generated, at the same time, a pair of datagram queue(input&ouput) will be push into related global datagrame queue manager.
 *
 * @param globalTime
 */
pissCommunicationStack::pissCommunicationStack(GlobalTime *globalTime, pissBaseDeDonnee* database){

    //! get global timer's pointer
    this->globalTime = globalTime;
    this->database = database;



    //! procedure to generate the architecture of the real time communication
    this->initComponents();
    this->initRealTimeTasks();
    this->setConnections();
    this->setReceptionTask();

}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::setConnections
//!
void pissCommunicationStack::setConnections(){
    this->connect(this->datagrammeAnalyser, SIGNAL(handshakeMessageReactProcess(QString,int)), this, SLOT(connectBackRequest(QString,int)));
}

//!--------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::setReceptionTask
//!
void pissCommunicationStack::setReceptionTask(){
    qRegisterMetaType<SOCKET>("SOCKET");
    this->connect(this->server, SIGNAL(incomingConnection(SOCKET)), this, SLOT(newReceptionTask(SOCKET)));
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::initComponents
//!
void pissCommunicationStack::initComponents(){
    this->devices = new pissDevices();
    this->devices->setMyListenPort(10703);
    this->database->setDevices(this->devices);
    this->connect(this->devices, SIGNAL(update()), database, SLOT(updateDevices()));
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::initRealTimeTasks
//!
void pissCommunicationStack::initRealTimeTasks(){
    this->datagrammeAnalyser = new pissDatagrammeAnalyser(&outputQueueManager, &inputQueueManager, devices, globalTime, database);
    this->informationDecodeTask = new pissInputInformationDecoder(&inputQueueManager, devices, datagrammeAnalyser);
    this->outputInformationEncoder = new pissOutputInformationEncoder(&outputQueueManager, devices, datagrammeAnalyser);
    this->server = new pissServer(&inputQueueManager, &outputQueueManager, devices, datagrammeAnalyser, globalTime, database);
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::connectBackRequest
//! \param ip
//! \param port
//!
void pissCommunicationStack::connectBackRequest(QString ip, int port){
    qDebug()<<"connectBackRequest"<<ip<<port;
    connectBack(false, ip, port);
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::setDatabase
//! \param database
//!
void pissCommunicationStack::setDatabase(pissBaseDeDonnee* database){

}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::clearBuffer
//!
void pissCommunicationStack::clearBuffer(){
    inputQueueManager.clear();
    outputQueueManager.clear();
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::closeServer
//! \return
//!
bool pissCommunicationStack::closeServer(){
    informationDecodeTask->stop();
    //outputInformationEncoder->stop();
    return server->stopServer();
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::launch
//!
bool pissCommunicationStack::launchServer(){
    informationDecodeTask->start();
    //outputInformationEncoder->start();
    return server->launchServer();
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::connectBack
//! \param flag
//! \param addr
//! \param port
//! \return
//!
bool pissCommunicationStack::connectBack(bool flag, QString addr, int port){
    if(flag){
        //! motivate connect
        //! int id  = devices->addClient();
        qDebug()<<"add client";
        pissClient *client = new pissClient(this->devices->getClientNumber(), &outputQueueManager, devices, globalTime, addr, port);
        this->datagrammeAnalyser->setConnectBackRequestWaitingPair(devices->getClientNumber(), client->connectRequest());
    }
    else{
        //! connect back process
        //! int id  = devices->addClient();
        pissClient *client = new pissClient(this->devices->getClientNumber()-1, &outputQueueManager, devices, globalTime, addr, port);
        //client->connectBackRequest(addr, port);
        this->datagrammeAnalyser->setConnectBackRequestWaitingPair(devices->getClientNumber(), client->connectBackRequest());
    }

    return true;
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissCommunicationStack::getNetworkEnvironment
//! \return
//!
pissDevice* pissCommunicationStack::getLocalDevice(){
    return this->devices->getLocalDevice();
}


void pissCommunicationStack::newReceptionTask(SOCKET sClient){


    //qDebug()<<"add incoming device";
    int id = this->devices->addClient();
    //this->devices->setSocketReceptionById(id, sClient);

    //qDebug()<<"generate correspondant input & output queue for the incoming device";
    pissInputQueue *myInputQueue = new pissInputQueue();

    //! push queue pair into global queue manager
    inputQueueManager.push_back(myInputQueue);

    //! create real time reception task
    pissReceptionTask *receptionTask = new pissReceptionTask(id, sClient, datagrammeAnalyser, &inputQueueManager, devices);
    receptionTask->start();

    return;
}
