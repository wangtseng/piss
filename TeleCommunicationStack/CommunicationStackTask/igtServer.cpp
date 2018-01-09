#include "igtServer.h"

/**
 * @brief igtServer::igtServer
 *
 *
 *
 */
igtServer::igtServer(igtInputQueueManager *inputQueueManager)
{
    if(inputQueueManager == NULL){
        return;
    }
    this->isConnected = false;
    this->inputQueueManager = inputQueueManager;
    this->incomingConnectionCount = 0;
}

//!------------------------------------------------------------------------------------------------------
//!
//! \brief igtServer::getIncomingConnectionCount
//! \return
//!
int igtServer::getIncomingConnectionCount(){
    return this->incomingConnectionCount;
}

//!------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::~pissServer
//!
igtServer::~igtServer(){

}

//!------------------------------------------------------------------------------------------------------
//!
//! \brief igtServer::launchServer
//! \return
//!
bool igtServer::launchServer(){
    QString ret = ipDetect();
    if(isConnected){
        QMessageBox msgBox;
        msgBox.setText("Server already in listening...");
        msgBox.exec();
        return false;
    }
    isConnected = this->listen(QHostAddress::Any, 10704);
    qDebug()<<"launchServer isConnected: "<<isConnected;
    emit localIPDetect(ret);
    return isConnected;
}

//!------------------------------------------------------------------------------------------------------
//!
//! \brief igtServer::stopServer
//! \return
//!
bool igtServer::stopServer(){
    if(isConnected){
        //! TODO stop task related and queues...........
        this->close();
        this->isConnected = false;
        return true;
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("server do not need to stop, hasn't been launched");
        msgBox.exec();
        return false;
    }

    return false;
}

//!------------------------------------------------------------------------------------------------------
//!
//! \brief igtServer::deleteServerThread
//!
void igtServer::stopServerThread(){
    //! parse receptionTaskManager and stop it
    if(receptionTaskManager.size() == 0){
        return;
    }

    for(unsigned char cpt = 0; cpt < receptionTaskManager.size(); cpt++){
        receptionTaskManager.at(0)->stop();
        receptionTaskManager.clear();
//        igtReceptionTask *ret = receptionTaskManager.at(0);
//        receptionTaskManager.pop_front();
        //delete ret;
    }
}

//!------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtServer::incomingConnection
//! \param handle
//!
void igtServer::incomingConnection(qintptr handle){
    this->incomingConnectionCount++;

    igtInputQueue *inputQueue = new igtInputQueue();
    this->inputQueueManager->addIgtInputQueue(inputQueue);

    igtReceptionTask *thread = new igtReceptionTask(handle, inputQueue);
    receptionTaskManager.append(thread);

    thread->start();
}

//! -------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtServer::setMode
//! \param mode
//!
void igtServer::setMode(int mode){
    this->mode = mode;
}

//! ------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtServer::ipDetect
//! \return
//!
QString igtServer::ipDetect(){
    QString ret;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int nIter = 0; nIter < list.count(); nIter++){
        if(list[nIter].protocol() == QAbstractSocket::IPv4Protocol && list[nIter] != QHostAddress(QHostAddress::LocalHost)){
            ret = list[nIter].toString();
            if(ret.contains("172")){
                break;
            }
        }
    }
    return ret;
}
