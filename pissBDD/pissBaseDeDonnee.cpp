#include "pissBaseDeDonnee.h"


/**
 * @brief SystemDataBase::SystemDataBase
 */
pissBaseDeDonnee::pissBaseDeDonnee(){
    this->patients = new Patients();
    this->metaData = new SystemMetaData();
    this->carm = new CArm();

}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::setImageProcessingFactory
//! \param imageProcessingFactory
//!
void pissBaseDeDonnee::setImageProcessingFactory(ImageProcessingFactory* imageProcessingFactory){
    this->imageProcessingFactory = imageProcessingFactory;
    this->patients->setImageProcessingFactory(imageProcessingFactory);
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getPatientsStatus
//! \return
//!
QStringList pissBaseDeDonnee::getPatientsStatus(){
    return this->patients->getPatientsStatus();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getPatientMRAImageById
//! \param id
//! \return
//!
IgssImage* pissBaseDeDonnee::getPatientMRAImageById(int id){
    return this->patients->getPatientMRAImageById(id);
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::setPaths
//! \param workspace_path
//! \param patients_path
//! \param metadata_path
//! \param image_center_path
//!
void pissBaseDeDonnee::setPaths(QString workspace_path,
                              QString patients_path,
                              QString metadata_path,
                              QString image_center_path){

    this->metaData->setSystemPaths(workspace_path,  patients_path,  metadata_path, image_center_path);
    this->patients->setPatientsPath(patients_path);
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getPatientByPesonnelInfo
//! \param name
//! \param birth
//! \return
//!
Patient* pissBaseDeDonnee::getPatientById(int id){
    patientHandling = this->patients->getPatientById(id);
    return patientHandling;
}

//! -------------------------------------------------------------------------------------------
//!
//! \brief pissBaseDeDonnee::getPatientHandling
//! \return
//!
Patient* pissBaseDeDonnee::getPatientHandling(){
    return patientHandling;
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief pissBaseDeDonnee::setPatientHandling
//! \param patientHandlingID
//!
void pissBaseDeDonnee::setPatientHandling(int patientHandlingID){
    //this->database->setPatientHandling(patientHandlingID);
    this->patientHandlingID = patientHandlingID;
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getSystemWorkSpace
//! \return
//!
QString pissBaseDeDonnee::getSystemWorkSpace(){
    return this->metaData->getSystemWorkSpace();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getPatientsPath
//! \return
//!
QString pissBaseDeDonnee::getPatientsPath(){
    return this->metaData->getPatientsPath();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getMetaData
//! \return
//!
QString pissBaseDeDonnee::getMetaData(){
    return this->metaData->getMetaData();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getImageCenterPath
//! \return
//!
QString pissBaseDeDonnee::getImageCenterPath(){
    return this->metaData->getImageCenterPath();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::findPatientsExisted
//!
int pissBaseDeDonnee::findPatientsExisted(){
    return this->patients->findPatientExisted();
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::getPatientsData
//! \return
//!
Patients* pissBaseDeDonnee::getPatientsData(){
    return this->patients;
}

//!--------------------------------------------------------------------------------------------
//!
//! \brief SystemDataBase::updateDevices
//!
void pissBaseDeDonnee::updateDevices(){
    qDebug()<<"updateDevices";
}
