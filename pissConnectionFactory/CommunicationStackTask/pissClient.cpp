#include "pissClient.h"


/**
 * @brief igtClient::igtClient
 * @param oq
 * @param devices
 */
pissClient::pissClient(int id, QVector <pissOutputQueue*> *oq, pissDevices* devices, GlobalTime *globalTime, QString addr, int port){

    this->id = id;
    this->oq = oq;
    this->devices = devices;
    this->globalTime = globalTime;
    this->motivateConnectionRequest = true;
    this->addr = addr;
    this->port = port;

    init();

    pissOutputQueue *myOutputQueue = new pissOutputQueue();
    //this->outputQueue = new OutputQueue();
    this->oq->push_back(myOutputQueue);
    //this->oq->append(this->outputQueue);


//    this->connect(this->connectconnectSocketket, SIGNAL(connected()), this, SLOT(startTransfer()));
//    this->connect(this->connectSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

//!----------------------------------------------------------------------------------
//!
//! \brief igtClient::init
//!
void pissClient::init(){

    connectSocket = INVALID_SOCKET;

    //! create WSADATA object
    WSADATA wsaData;

    //! Initialize Winsock
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    if(iResult != 0){
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }
}

//! ---------------------------------------------------------------------------------
//!
//! \brief igtClient::connect_request
//! \param addr
//! \param port
//!
SOCKET pissClient::connectRequest(){
    this->motivateConnectionRequest = true;

    //! transform ip and port to char*
    QByteArray addr1 = addr.toLatin1();
    char *addr2 = addr1.data();
//    qDebug()<<"addr2"<<addr2;

    QString temp = QString::number(port, 10);
    QByteArray temp1 = temp.toLatin1();
    const char *port_str = temp1.data();
//    qDebug()<<"port_str"<<port_str;

    //! holds address info for socket to connect to
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    //! set address info
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;    //! TCP connection


    //! resolve server address and port
    int iResult;
    iResult = getaddrinfo((PCSTR)addr2, (PCSTR)port_str, &hints, &result);

    if(iResult != 0){
        printf("getaddrinfo failed with error: %d\n", iResult);
        exit(1);
    }

    //! Attempt to connect to an address until one succeeds
    for(ptr = result; ptr != NULL; ptr = ptr->ai_next){


        //! Create a SOCKET for connecting to server
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if(INVALID_SOCKET == connectSocket){
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            exit(1);
        }

        qDebug()<<"set socket";

        //! Connect to server
        iResult = ::connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        qDebug()<<"connect iResult"<<iResult;
        if(iResult == SOCKET_ERROR){
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            printf("The server is down... did not connect\n");
        }
    }

    //! no longer need address info for server
    freeaddrinfo(result);


    //! if connection failed
    if(connectSocket == INVALID_SOCKET){
        printf("Unable to connect to server!\n");
        WSACleanup();
        exit(1);
    }


    //! Set the mode of the socket to be nonblocking
    u_long iMode = 1;

    iResult = ioctlsocket(connectSocket, FIONBIO, &iMode);
    if(iResult == SOCKET_ERROR){
        printf("ioctlsocket failed with error :%d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        exit(1);
    }

    //! disable nagle
    char value = 1;
    setsockopt(connectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

    //! generate correspondance transmission task
    this->transmissionTask = new pissTransmissionTask(this->id, this->oq, devices, connectSocket);

    this->startTransfer();

    return this->connectSocket;

//    this->connectSocket->connectToHost(addr, pnort);
//    return this->connectSocket->socketDescriptor();
}


//! ---------------------------------------------------------------------------------
//!
//! \brief igtClient::connectBackRequest
//! \param addr
//! \param port
//!
SOCKET pissClient::connectBackRequest(){
    this->motivateConnectionRequest = false;

    //! transform ip and port to char*
    QByteArray addr1 = addr.toLatin1();
    char *addr2 = addr1.data();
    QString temp = QString::number(port, 10);
    QByteArray temp1 = temp.toLatin1();
    const char *port_str = temp1.data();

    //! holds address info for socket to connect to
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    //! set address info
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;    //! TCP connection

    //! resolve server address and port
    int iResult;
    iResult = getaddrinfo((PCSTR)addr2, (PCSTR)port_str, &hints, &result);

    if(iResult != 0){
        printf("getaddrinfo failed with error: %d\n", iResult);
        exit(1);
    }

    //! Attempt to connect to an address until one succeeds
    for(ptr = result; ptr != NULL; ptr = ptr->ai_next){


        //! Create a SOCKET for connecting to server
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if(INVALID_SOCKET == connectSocket){
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            exit(1);
        }

        //! Connect to server
        iResult = ::connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        if(iResult == SOCKET_ERROR){
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            printf("The server is down... did not connect\n");
        }
    }

    //! no longer need address info for server
    freeaddrinfo(result);

    //! if connection failed
    if(connectSocket == INVALID_SOCKET){
        printf("Unable to connect to server!\n");
        WSACleanup();
        exit(1);
    }

    //! Set the mode of the socket to be nonblocking
    u_long iMode = 1;

    iResult = ioctlsocket(connectSocket, FIONBIO, &iMode);
    if(iResult == SOCKET_ERROR){
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        exit(1);
    }

    //! disable nagle
    char value = 1;
    setsockopt(connectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

    //! generate correspondance transmission task
    this->transmissionTask = new pissTransmissionTask(this->id, this->oq, devices, connectSocket);

    this->startTransfer();

    return this->connectSocket;


//    this->connectSocket->connectToHost(addr, port);
//    return this->connectSocket->socketDescriptor();
}


//! ---------------------------------------------------------------------------------
//!
//! \brief igtClient::slotDisconnected
//!
void pissClient::slotDisconnected(){
    qDebug()<<"disconnected";
}

//! ---------------------------------------------------------------------------------
//!
//! \brief igtClient::startTransfer
//!
void pissClient::startTransfer(){
    //qDebug()<<"igtClient start transfer using socket: "<<this->connectSocket->socketDescriptor();

    //devices->setSocketTransById(id, connectSocket->socketDescriptor());

    if(motivateConnectionRequest){
        pissHandShakeMessage *msg = new pissHandShakeMessage();

        msg->setDataType(1);
        msg->setOrigineId(1);
        msg->setTargetId(2);
        msg->setTimeStampes(globalTime->currentTime());
        msg->setDLC(128);

//        qDebug()<<"address:"<<this->devices->getLocalDevice()->getIpAddress();
//        qDebug()<<"port"<<this->devices->getLocalDevice()->getClientlistenport();

        msg->setAddr(this->devices->getLocalDevice()->getIpAddress());
        msg->setPort(this->devices->getLocalDevice()->getClientlistenport());

//        msg->print();

        pissCDatagramme *datagramme = new pissCDatagramme();
        datagramme->toCDatagram(msg);

//        datagramme->print();
        this->oq->at(id)->append(datagramme);

    }
    else{
        pissHandShakeCommitMessage *msg = new pissHandShakeCommitMessage();

        msg->setDataType(2);
        msg->setOrigineId(2);
        msg->setTargetId(2);
        msg->setTimeStampes(globalTime->currentTime());
        msg->setDLC(38);
        msg->setAddr(this->devices->getLocalDevice()->getIpAddress());
        msg->setPort(this->devices->getLocalDevice()->getClientlistenport());

        pissCDatagramme *datagramme = new pissCDatagramme();
        datagramme->toCDatagram(msg);
//        qDebug()<<dg.getDataType();

        qDebug()<<"transport";
        datagramme->print();

        this->oq->at(id)->append(datagramme);
//        qDebug()<<"hand shake commit push into :"<<id;
    }

    transmissionTask->launch();

}


