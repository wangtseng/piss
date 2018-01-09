#include "pissServer.h"


/**
 * @brief pissServer::pissServer
 * @param inputQueueManager
 * @param NetworkEnvironment
 * @param datagrammeAnalyser
 */
pissServer::pissServer(QVector <pissInputQueue*> *inputQueueManager,
                       QVector <pissOutputQueue*> *outputQueueManager,
                       pissDevices *networkEnvironment,
                       pissDatagrammeAnalyser *datagrammeAnalyser,
                       GlobalTime *globalTime,
                       pissBaseDeDonnee* database)
{
    this->flag = true;

    this->serverStatus = false;

    this->inputQueueManager = inputQueueManager;
    this->outputQueueManager = outputQueueManager;
    this->devices = networkEnvironment;
    this->datagrammeAnalyser = datagrammeAnalyser;
    this->globalTime = globalTime;
    this->database = database;

    this->init();

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::~pissServer
//!
pissServer::~pissServer(){

}

bool pissServer::init(){
    this->socketServer = INVALID_SOCKET;

    //! create WSADATA
    WSADATA wsaData;

    //! initialize Winsock
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), (LPWSADATA)&wsaData);

    if(iResult != 0){
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }


    return true;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::stopServer
//! \return
//!
bool pissServer::stopServer(){
    if(serverStatus){

        //! TODO stop task related and queues...........

        //this->close();
        closesocket(this->socketServer);
        //WSACleanup();
        serverStatus = false;
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


//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::launchServer
//!
bool pissServer::launchServer(){

    if(serverStatus){
        QMessageBox msgBox;
        msgBox.setText("Server already in listening...");
        msgBox.exec();
        return false;
    }

    //! address info for the server to listen to
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    //! set address information
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;    //! Tcp connection
    hints.ai_flags = AI_PASSIVE;

    //! Resolve the server address and port
    QString temp = QString::number(this->devices->getMyListenPort(), 10);
    QByteArray temp1 = temp.toLatin1();
    const char *port_str = temp1.data();

    int iResult;
    iResult = getaddrinfo(NULL, port_str, &hints, &result);

    if(iResult != 0){
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    //! Create a SOCKET for connecting to server
    socketServer =socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(SOCKET_ERROR == socketServer){
        printf("socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    //! set the mode of the socket to be nonblocking
    u_long iMode = 1;

    iResult = ioctlsocket(socketServer, FIONBIO, &iMode);
    if(iResult == SOCKET_ERROR){
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(socketServer);
        WSACleanup();
        exit(1);
    }

    //! bind
    if(SOCKET_ERROR == bind(socketServer, result->ai_addr, (int)result->ai_addrlen)){
        printf("Bind error\n");
        freeaddrinfo(result);
        closesocket(socketServer);
        WSACleanup();
        exit(1);
    }

    //! no longer need address information
    freeaddrinfo(result);
    //! listen
    if(SOCKET_ERROR == listen(socketServer, 4)){
        printf("Listen error\n");
        closesocket(socketServer);
        WSACleanup();
        exit(1);
    }

    this->start();
    this->serverStatus = true;
    return serverStatus;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::getConnectionState
//!
bool pissServer::getConnectionState(){
    return serverStatus;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissServer::incomingConnection
//! \param sd
//!
void pissServer::run(){
    while(flag){

        SOCKET sClient;
        if(INVALID_SOCKET == (sClient = ::accept(this->socketServer, NULL, NULL))){
            continue;
        }

        qDebug()<<"accept client";

        //! disable nagle
        char value = 1;
        setsockopt(sClient, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

        //! set the mode of the socket to be nonblocking
        u_long iMode = 1;
        int iResult = ioctlsocket(sClient, FIONBIO, &iMode);
        if(iResult == SOCKET_ERROR){
            printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
            closesocket(sClient);
            WSACleanup();
            exit(1);
        }
        emit incomingConnection(sClient);
    }
}










