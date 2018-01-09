#include "pissDevice.h"


/**
 * @brief Device::Device
 */
pissDevice::pissDevice(){

}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::setModuleNumber
//! \param moduleNumber
//!
void pissDevice::setId(int id){
    this->id = id;
    switch (id){
        case PISS:
            setName("piss");
            break;
        case CCISS:
            setName("cciss");
            break;
        case MISS:
            setName("miss");
            break;
        default:
            break;
    }
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::getIpAddress
//! \return
//!
QString pissDevice::getIpAddress()
{
    return this->addr;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::getPort
//! \return
//!
int pissDevice::getPortNumber()
{
    return this->portNumber;
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::getModuleNumber
//! \return
//!
int pissDevice::getDeviceNumber(){
    return this->id;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::getName
//! \return
//!
QString pissDevice::getName()
{
    return this->name;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::getSocketrec
//! \return
//!
int pissDevice::getSocketrec()
{
    return this->socketrec;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::getSocketTrans
//! \return
//!
int pissDevice::getSockettrans()
{
    return this->sockettrans;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::getClientlistenport
//! \return
//!
quint32 pissDevice::getClientlistenport(){
    return this->clientlistenport;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::getCode
//! \return
//!
bool pissDevice::getCode(){
    return this->code;
}



//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::setIpAddress
//! \param Addr
//!
void pissDevice::setIpAddress(QString addr){
    qDebug()<<addr;
    this->addr = addr;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::setPort
//! \param Port
//!
void pissDevice::setPortNumber(int p){
    this->portNumber = p;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::setName
//! \param Name
//!
void pissDevice::setName(QString Name){
    this->name = Name;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::setSocketrec
//! \param Socketrec
//!
void pissDevice::setSocketrec(SOCKET Socketrec){
    this->socketrec = Socketrec;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::setSocketTrans
//! \param SocketTrans
//!
void pissDevice::setSockettrans(SOCKET Sockettrans){
    this->sockettrans = Sockettrans;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::setClientlistenport
//! \param Clientlistenport
//!
void pissDevice::setClientlistenport(quint32 Clientlistenport){
    this->clientlistenport = Clientlistenport;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ModuleIpConfig::setCode
//! \param Code
//!
void pissDevice::setCode(bool Code){
    this->code = Code;
}
