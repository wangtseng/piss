#include "PersonnelImageLabel.h"


PersonnelImageLabel::PersonnelImageLabel(){
    this->setAcceptDrops(true);
}

PersonnelImageLabel::~PersonnelImageLabel(){

}

QString PersonnelImageLabel::getImagePath(){
    return this->imgPath;
}

void PersonnelImageLabel::dragEnterEvent(QDragEnterEvent *event){
     event->acceptProposedAction();
}

void PersonnelImageLabel::dropEvent(QDropEvent *event){
    QList<QUrl> l= event->mimeData()->urls();
    if(l.size() == 1){
        this->imgPath = l[0].toLocalFile();
        this->setPixmap(QPixmap(this->imgPath));
        this->setAutoFillBackground(true);
        this->setScaledContents(true);
        this->update();
    }
    else{
        qDebug()<<"trop de photo.....";
    }
}
