#include "SystemDispatcher.h"


/**
 * @brief SystemDispatcher::SystemDispatcher
 *
 *
 *
 */
SystemDispatcher::SystemDispatcher(){
    this->initializeSystemPaths();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::getPatientsStatus
//! \return
//!
QStringList SystemDispatcher::getPatientsStatus(){
    return this->database->getPatientsStatus();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::extractBrainCortextFrom
//! \param input
//! \param output
//!
bool SystemDispatcher::extractBrainCortextFrom(vtkImageData *input, QString type, QString path){
    return this->imageProcessingFactory->extractBrainCortextFrom(input, type, path);
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::extractHistogramStatistics
//! \param img
//! \return
//!
QStringList SystemDispatcher::extractHistogramStatistics(vtkImageData *img){
    return this->imageProcessingFactory->getHistogramStatisticsFrom(img);
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::getHistogramOfVolumeData
//! \param img
//! \param points
//!
QVector<HistogramPoint*> SystemDispatcher::getHistogramOfVolumeData(vtkImageData *img){
   return this->imageProcessingFactory->getHistogramFrom(img);
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::getPatientInDataBase
//! \param name
//! \param birth
//! \return
//!
Patient* SystemDispatcher::getPatientById(int id){
    return this->database->getPatientById(id);
}

//! ---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::setPatientHandling
//! \param patientHandlingID
//!
void SystemDispatcher::setPatientHandling(int patientHandlingID){
    this->database->setPatientHandling(patientHandlingID);
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::startPissServer
//!
bool SystemDispatcher::launchCommunicationStackServer(){
    return 0;//communicationStack->launchServer();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::launchConnectBackProcess
//! \param flag
//! \param addr
//! \param port
//!
void SystemDispatcher::launchConnectBackProcess(bool flag, QString addr,int port){
    //this->communicationStack->connectBack(flag, addr, port);
}


//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::stopPissServer
//! \return
//!
bool SystemDispatcher::stopPissServer(){
    return 0;//communicationStack->closeServer();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::setMessageDisplayWindow
//! \param msgDisplay
//!
void SystemDispatcher::setMessageDisplayWindow(QTextEdit *msgDisplay){

}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief get_username
//! \return
//!
QString SystemDispatcher::get_username(){
    QString name = qgetenv("USER");
        if (name.isEmpty())
            name = qgetenv("USERNAME");
    return name;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::initializeSystemPaths
//!
void SystemDispatcher::initializeSystemPaths(){

    //! ------create some folders by predefined paths as the workspaces of the application-------------------
    QString username = get_username();

    if(username != "unknown") {
        //! define the software's deault path according to the os type

        #ifdef linux
        workspace_path = "/home/" + username + "/Documents/CanalyserWorkspace/";
        patients_path = workspace_path + "PatientsDataware/";
        metadata_path = workspace_path + "CanalyserMetadata/";
        image_center_path = workspace_path + "CanalyserImageCenter/";
        #elif _WIN64
        workspace_path = "C:\\Users\\" + username + "\\Documents\\CanalyserWorkspace\\";
        patients_path = workspace_path + "PatientsDataware\\";
        metadata_path = workspace_path + "CanalyserMetadata\\";
        image_center_path = workspace_path + "CanalyserImageCenter\\";
        #elif __APPLE__
        workspace_path = "/Users/" + username + "/Documents/CanalyserWorkspace/";
        patients_path = workspace_path + "PatientsDataware/";
        metadata_path = workspace_path + "CanalyserMetadata/";
        image_center_path = workspace_path + "CanalyserImageCenter/";
        #endif
    }
    else{
        //qDebug()<<"exception";
        return;
    }

    QFileInfo folder(workspace_path);

    if(!folder.isDir()){
        QDir path;
        path.mkdir(workspace_path);
        path.mkdir(patients_path);
        path.mkdir(metadata_path);
        path.mkdir(image_center_path);
    }
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::setSystemDataBase
//! \param database
//!
void SystemDispatcher::setSystemDataBase(pissBaseDeDonnee* database){
    this->database = database;
    this->database->setPaths(this->workspace_path,
                             this->patients_path,
                             this->metadata_path,
                             this->image_center_path);
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::setImageProcessingFactory
//! \param imageProcessingFactory
//!
void SystemDispatcher::setImageProcessingFactory(ImageProcessingFactory *imageProcessingFactory){
    this->imageProcessingFactory = imageProcessingFactory;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::setCommunicationStack
//! \param communicationStack
//!
void SystemDispatcher::setCommunicationStack(igtCommunicationStack *communicationStack){
    this->communicationStack = communicationStack;
    this->connect(this->communicationStack, SIGNAL(localIPDetect(QString)), this, SLOT(getSelfIp(QString)));
    this->connect(this->communicationStack, SIGNAL(newConnection(QString)), this, SLOT(connectBack(QString)));
}

void SystemDispatcher::setOmega3(Omega3 *omega){
    this->omega = omega;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::launchOmega
//! \param patientHandling
//!
void SystemDispatcher::launchOmega(Patient*patientHandling){
    this->omega->setPatientHandling(patientHandling);
    this->omega->launch();
}

void SystemDispatcher::terminateOmega(){
    this->omega->terminate();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::getImageCenterPath
//! \return
//!
QString SystemDispatcher::getImageCenterPath(){
     return this->image_center_path;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::findPatientExisted
//!
int SystemDispatcher::findPatientExisted(){
    return this->database->findPatientsExisted();
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::doImageProcessingByMethodType
//! \param id
//! \param dim
//! \param method
//! \return
//!
QString SystemDispatcher::doImageProcessingByMethodType(int id, int dim, QString method){
    QString msg;
    output = new IgssImage();

    IgssImage*input = this->database->getPatientMRAImageById(id);

    eProcessingErrorCode ret = this->imageProcessingFactory->doProcessingByCommand(input,output,dim, method);

    if(ret == PROCESSING_NO_ERRROR){
        msg= method + " proceed with no error";
    }

    return msg;
}

QStringList SystemDispatcher::getHistogramStatisticsFrom(vtkImageData* mraImage){
    return this->imageProcessingFactory->getHistogramStatisticsFrom(mraImage);
}

//void SystemDispatcher::getLatestProgressSpeed(double *speed){
//    return this->database->getLatestProgressSpeed(speed);
//}

//void SystemDispatcher::clearAllBuffers(){
//    this->database->clearAllBuffers();
//}

int SystemDispatcher::getIncomingConnectionCount(){
    return communicationStack->getIncomingConnectionCount();
}

bool SystemDispatcher::lauchServer(){
    bool ret;
    ret = this->communicationStack->lauchServer();
    return ret;
}

bool SystemDispatcher::stopServer(){
    bool ret;
    if(this->communicationStack == NULL){
        return false;
    }
    ret = this->communicationStack->stopServer();
    return ret;
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::connectera
//! \param targetIpLineEdit
//! \param targetPortLineEdit
//! \param MB
//! \return
//!
bool SystemDispatcher::connectera(QString targetIpLineEdit,QString targetPortLineEdit, bool MB){
    return this->communicationStack->connectera(targetIpLineEdit, targetPortLineEdit, MB);
}

void SystemDispatcher::getSelfIp(QString IP){
    QString ret = IP;
    emit localIPDetect(ret);
}

void SystemDispatcher::connectBack(QString remoteIP){
    QString ret = remoteIP;
//    this->connectera(ret, "10703");
    emit newConnection(ret);
}
