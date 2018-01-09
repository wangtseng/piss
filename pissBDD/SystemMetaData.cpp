#include "SystemMetaData.h"

SystemMetaData::SystemMetaData()
{

}

void SystemMetaData::setSystemPaths(QString workspace_path,  QString patients_path, QString metadata_path, QString image_center_path){
    this->workspace_path = workspace_path;
    this->patients_path = patients_path;
    this->metadata_path = metadata_path;
    this->image_center_path = image_center_path;
}

QString SystemMetaData::getSystemWorkSpace(){
    return this->workspace_path;
}

QString SystemMetaData::getPatientsPath(){
    return this->patients_path;
}

QString SystemMetaData::getMetaData(){
    return this->metadata_path;
}

QString SystemMetaData::getImageCenterPath(){
    return this->image_center_path;
}
