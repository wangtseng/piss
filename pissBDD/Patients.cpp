#include "Patients.h"

Patients::Patients(){
    id = 0;
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief Patients::setImageProcessingFactory
//! \param imageProcessingFactory
//!
void Patients::setImageProcessingFactory(ImageProcessingFactory* imageProcessingFactory){
    this->imageProcessingFactory = imageProcessingFactory;
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief Patients::getPatientsStatus
//! \return
//!
QStringList Patients::getPatientsStatus(){
    QStringList statusInfo;
    for(unsigned char cpt = 0; cpt < patients.size(); cpt++){
        if((patients.at(cpt)->readFinished())){
            statusInfo<<"patient "+QString::number(patients.at(cpt)->getIdNumber())+
                        " mra image lecture finished";
        }
    }
    return statusInfo;
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief Patients::getPatientByPesonnelInfo
//! \param name
//! \param birth
//! \return
//!
Patient* Patients::getPatientById(int id){
    for(unsigned char cpt = 0; cpt < patients.size(); cpt++){
        if((patients.at(cpt)->getIdNumber() == id)){
            return patients.at(cpt);
        }
    }
    return NULL;
}



//!----------------------------------------------------------------------------------------------------
//!
//! \brief Patients::findPatientExisted
//!
int Patients::findPatientExisted(){
    //qDebug()<<this->patients_path;
    QDir patientsPath(this->patients_path);

    patientsPath.setFilter(QDir::Dirs);

    QFileInfoList list = patientsPath.entryInfoList();

    for(unsigned char cpt = 0; cpt < list.size(); cpt++){
        if(list.at(cpt).filePath().contains(".") || list.at(cpt).filePath().contains("..")){
            continue;
        }
        //qDebug()<<list.at(cpt).filePath();
        Patient *patient = new Patient(list.at(cpt).filePath(), id);
        patient->setImageProcessingFactory(this->imageProcessingFactory);
        patients.append(patient);
        id++;
    }


    return id;
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief Patients::setPatientsPath
//! \param patients_path
//!
void Patients::setPatientsPath(QString patients_path){
    this->patients_path = patients_path;
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief Patients::getPatientMRAImageById
//! \param id
//! \return
//!
IgssImage* Patients::getPatientMRAImageById(int id){
    return this->patients.at(id)->getOriginImage();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief Patients::getPatientsList
//! \return
//!
QList<Patient *> Patients::getPatientsList(){
    return this->patients;
}
