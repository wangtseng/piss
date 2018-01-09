#include "pissAddToDatawareTask.h"


pissAddToDatawareTask::pissAddToDatawareTask(CollabIGTDataware *igtDataware)
{

    this->igtDataware = igtDataware;
}

void pissAddToDatawareTask::run(){
    
    QDir dir(this->filePath);
    if(!dir.exists()){
        return;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    
    int file_count = list.count();
    
    if(file_count <= 0){
        return;
    }
    
    QStringList string_list;
    for(int i = 0; i < file_count; i++){
        QFileInfo file_info = list.at(i);
        QString suffix = file_info.suffix();
        if(QString::compare(suffix, QString("raw"), Qt::CaseInsensitive) == 0){
            QString absolute_file_path = file_info.absoluteFilePath();
            string_list.append(absolute_file_path);
        }
    }
    
    for(int i = 0; i < string_list.size(); i++){
        CollabIGTCTImage *ctImage = new CollabIGTCTImage();

        ctImage->setWidth(1024);
        ctImage->setHeight(1024);
        ctImage->setDepth(1);
        ctImage->setXSpacing(5);
        ctImage->setYSpacing(5);
        ctImage->setZSpacing(5);
        ctImage->setDatatype(1);
        ctImage->setMSB(1);

        ctImage->doReadCTImageByPath(string_list.at(i));

        this->igtDataware->appendAnCollabIGTCTImage(ctImage);
    }
    
    return;
}
