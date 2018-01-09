#include "pissDevices.h"


/**
 * @brief Devices::Devices
 *
 * the class which represent all the module in this distributed system under the protocol (a definir a la fin)
 *
 * include not only incomming devices, and also local device
 *
 * each time, a new incomming devices via protocol tcp/ip, an Device object will be instantiate here.
 *
 */
pissDevices::pissDevices(){

    this->incommingDeviceCount = 0;

    this->localDevice = new pissDevice();
    this->localDevice->setIpAddress(ipDetect());
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Devices::getSelf
//! \return
//!
pissDevice* pissDevices::getLocalDevice(){
    return localDevice;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Devices::setMyListenPort
//! \param port
//!
void pissDevices::setMyListenPort(quint32 port){
    localDevice->setClientlistenport(port);
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Devices::setSocketReceptionById
//! \param id
//! \param socketRec
//!
void pissDevices::setSocketReceptionById(int id, SOCKET sr){
    this->incomingDevices.at(id)->setSocketrec(sr);
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::addAClient
//!
int pissDevices::addClient(){
    pissDevice* device = new pissDevice();
    device->setId(this->incommingDeviceCount);
    mutex.lock();
    this->incomingDevices.append(device);
    mutex.unlock();
    this->incommingDeviceCount++;
    return (this->incommingDeviceCount-1);
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::getClientNumber
//! \return
//!
int pissDevices::getClientNumber(){
    return this->incommingDeviceCount;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::selfDetectIP
//! \return
//!
QString pissDevices::ipDetect(){
    QString ret;

    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    for(int nIter=0; nIter<list.count(); nIter++){
      if(!list[nIter].isLoopback()){
          if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol ){

              ret = list[nIter].toString();
              if(ret.contains("172")){
                  break;
              }
          }
      }
    }

    return ret;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::init
//! \param Module
//! \param Port
//!
void pissDevices::append(QString deviceName, int Port)
{
    if(deviceName == "self")
    {
        pissDevice* selfIpConfig = new pissDevice();
        selfIpConfig->setIpAddress(this->ipDetect());
        selfIpConfig->setPortNumber(Port);
        selfIpConfig->setName(deviceName);
        selfIpConfig->setCode(0);

        mutex.lock();
        this->incomingDevices.append(selfIpConfig);
        mutex.unlock();
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::append
//! \param ModuleNumber
//! \param Port
//!
void pissDevices::append(int id, int port){
    pissDevice* device = new pissDevice();
    device->setIpAddress(ipDetect());
    device->setPortNumber(port);
    device->setId(id);
    device->setSockettrans(0);
    device->setSocketrec(0);
    device->setCode(0);
    mutex.lock();
    this->incomingDevices.append(device);
    mutex.unlock();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::init
//! \param ModuleName
//! \param Port
//! \param addr
//! \param Socketrec
//! \param Clientlistenport
//! \param Code
//!
void pissDevices::append(QString ModuleName,
                                    int Port,
                                    QString addr,
                                    int Socketrec,
                                    quint32 Clientlistenport,
                                    quint8 Code)
{
    mutex.lock();
    pissDevice *missIpConfig = new pissDevice();
    missIpConfig->setIpAddress(addr);
    missIpConfig->setPortNumber(Port);
    missIpConfig->setName(ModuleName);
    missIpConfig->setSocketrec(Socketrec);
    missIpConfig->setClientlistenport(Clientlistenport);
    missIpConfig->setCode(Code);
    this->incomingDevices.append(missIpConfig);
    mutex.unlock();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::append
//! \param ModuleNumber
//! \param Port
//! \param addr
//! \param Socketrec
//! \param Clientlistenport
//! \param Code
//!
void pissDevices::append(int ModuleNumber,
                                    int Port,
                                    QString addr,
                                    int Socketrec,
                                    quint32 Clientlistenport,
                                    quint8 Code){
    mutex.lock();
    QHostAddress address(addr);
    pissDevice *missIpConfig = new pissDevice();
    missIpConfig->setIpAddress(addr);
    missIpConfig->setPortNumber(Port);
    missIpConfig->setId(ModuleNumber);
    missIpConfig->setSocketrec(Socketrec);
    missIpConfig->setClientlistenport(Clientlistenport);
    missIpConfig->setCode(Code);
    this->incomingDevices.append(missIpConfig);
    mutex.unlock();
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief Devices::setSocketTransById
//! \param id
//! \param Sockettrans
//!
void pissDevices::setSocketTransById(int id, SOCKET st)
{
    mutex.lock();
    this->incomingDevices.at(id)->setSockettrans(st);
    mutex.unlock();
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief Devices::setIpAddressByModule
//! \param ModuleName
//! \param ip
//!
void pissDevices::setIpAddressById(int id, QString ip)
{
    mutex.lock();
    this->incomingDevices.at(id)->setIpAddress(ip);
    mutex.unlock();
}

void pissDevices::setClientlistenportById(int id, quint32 Clientlistenport)
{
    mutex.lock();
    this->incomingDevices.at(id)->setClientlistenport(Clientlistenport);
    mutex.unlock();
    emit update();
}

void pissDevices::setCodeByModule(QString ModuleName, bool Code)
{
    mutex.lock();
    for(unsigned char cpt = 0; cpt < this->incomingDevices.size(); cpt++)
    {
        if(this->incomingDevices.at(cpt)->getName() == ModuleName)
        {
            this->incomingDevices.at(cpt)->setCode(Code);
        }
    }
    mutex.unlock();
}




//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::getIpAddressByModule
//! \param moduleName
//! \return
//!
QHostAddress pissDevices::getIpAddressByModule(QString ModuleName)
{
    QHostAddress ret;
    mutex.lock();
    for(unsigned char cpt = 0; cpt < this->incomingDevices.size(); cpt++)
    {
        if(this->incomingDevices.at(cpt)->getName() == ModuleName)
        {
            ret =  this->incomingDevices.at(cpt)->getIpAddress();
        }
    }
    mutex.unlock();
    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::getIpAddressByModule
//! \param ModuleNumber
//! \return
//!
QHostAddress pissDevices::getIpAddressByModule(int ModuleNumber){
    QHostAddress ret;
    mutex.lock();
    for(unsigned char cpt = 0; cpt < this->incomingDevices.size(); cpt++)
    {
        if(this->incomingDevices.at(cpt)->getDeviceNumber() == ModuleNumber)
        {
            ret =  this->incomingDevices.at(cpt)->getIpAddress();
        }
    }
    mutex.unlock();
    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::getPortByModule
//! \param moduleName
//! \return
//!
int pissDevices::getPortByModule(QString ModuleName)
{
    int ret = -1;
    mutex.lock();
    for(unsigned char cpt = 0; cpt < this->incomingDevices.size(); cpt++)
    {
        if(this->incomingDevices.at(cpt)->getName() == ModuleName)
        {
            ret = this->incomingDevices.at(cpt)->getPortNumber();
        }
    }
    mutex.unlock();
    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Devices::getMyListenPort
//! \return
//!
int pissDevices::getMyListenPort(){
    return this->localDevice->getClientlistenport();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::getPortByModule
//! \param ModuleNumber
//! \return
//!
int pissDevices::getPortByModule(int deviceNumber){
    int ret = -1;
    mutex.lock();
    for(unsigned char cpt = 0; cpt < this->incomingDevices.size(); cpt++)
    {
        if(this->incomingDevices.at(cpt)->getDeviceNumber() == deviceNumber)
        {
            ret = this->incomingDevices.at(cpt)->getPortNumber();
        }
    }
    mutex.unlock();
    return ret;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissNetworkEnvironment::getSocketrecByModule
//! \param ModuleName
//! \return
//!
int pissDevices::getSocketrecByModule(QString ModuleName)
{
    //mutex.lock();
    for(unsigned char cpt = 0; cpt < this->incomingDevices.size(); cpt++)
    {
        if(this->incomingDevices.at(cpt)->getName() == ModuleName)
        {
            return this->incomingDevices.at(cpt)->getSocketrec();
        }
    }
    //mutex.unlock();
}

int pissDevices::getSockettransByModule(QString ModuleName)
{
    mutex.lock();
    for(unsigned char cpt = 0; cpt < this->incomingDevices.size(); cpt++)
    {
        if(this->incomingDevices.at(cpt)->getName() == ModuleName)
        {
            return this->incomingDevices.at(cpt)->getSockettrans();
        }
    }
    mutex.unlock();
}

int pissDevices::getSockettransByCode(quint8 Code)
{
    mutex.lock();
    for(unsigned char cpt = 0; cpt < this->incomingDevices.size(); cpt++)
    {
        if(this->incomingDevices.at(cpt)->getCode() == Code)
        {
            return this->incomingDevices.at(cpt)->getSockettrans();
        }
    }
    mutex.unlock();
}

quint32 pissDevices::getClientlistenportByModule(QString ModuleName)
{
    mutex.lock();
    for(unsigned char cpt = 0; cpt < this->incomingDevices.size(); cpt++)
    {
        if(this->incomingDevices.at(cpt)->getName() == ModuleName)
        {
            return this->incomingDevices.at(cpt)->getClientlistenport();
        }
    }
    mutex.unlock();
}

bool pissDevices::getCodeByModule(QString ModuleName)
{
    mutex.lock();
    for(unsigned char cpt = 0; cpt < this->incomingDevices.size(); cpt++)
    {
        if(this->incomingDevices.at(cpt)->getName() == ModuleName)
        {
            return this->incomingDevices.at(cpt)->getCode();
        }
    }
    mutex.unlock();
}

pissDevice* pissDevices::getDeviceAt(int index){
    return incomingDevices.at(index);
}
