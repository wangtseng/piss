#include "CollabIGTCTImage.h"

//!---------------------------------------------------------------------------------
//!
//! \brief CollabIGTCTImage::CollabIGTCTImage
//! \param filepath
//!
CollabIGTCTImage::CollabIGTCTImage()
{
}


void CollabIGTCTImage::setWidth(int width){
    this->width = width;
}

int CollabIGTCTImage::getWidth(){
    int ret = this->width;
    return ret;
}


void CollabIGTCTImage::setHeight(int height){
    this->height = height;
}

int CollabIGTCTImage::getHeight(){
    int ret = this->height;
    return ret;
}


void CollabIGTCTImage::setDepth(int depth){
    this->depth = depth;
}

int CollabIGTCTImage::getDepth(){
    int ret = this->depth;
    return ret;
}


void CollabIGTCTImage::setXSpacing(int xSpacing){
    this->xSpacing = xSpacing;
}

int CollabIGTCTImage::getXSpacing(){
    int ret = this->xSpacing;
    return ret;
}


void CollabIGTCTImage::setYSpacing(int ySpacing){
    this->ySpacing = ySpacing;
}

int CollabIGTCTImage::getYSpacing(){
    int ret = this->ySpacing;
    return ret;
}


void CollabIGTCTImage::setZSpacing(int zSpacing){
    this->zSpacing = zSpacing;
}

int CollabIGTCTImage::getZSpacing(){
    int ret = this->zSpacing;
    return ret;
}


void CollabIGTCTImage::setDatatype(int datatype){
    this->datatype = datatype;
}

int CollabIGTCTImage::getDatatype(){
    int ret = this->datatype;
    return ret;
}


void CollabIGTCTImage::setMSB(int MSB){
    this->MSB = MSB;
}

int CollabIGTCTImage::getMSB(){
    int ret = this->MSB;
    return ret;
}


void CollabIGTCTImage::doReadCTImageByPath(QString CTImagePath){

    QFile *ctImageMessageFile = new QFile(CTImagePath);

    if(!ctImageMessageFile->open(QIODevice::ReadOnly | QIODevice::Truncate)){
        return;
    }

    QByteArray msg = ctImageMessageFile->readAll();

    //! convert msg from QByteArray into (const char*)
    ctImageMsgBody = new char(msg.size());
    const char *ret = msg.constData();
    memcpy(ctImageMsgBody, ret, msg.size());
    ctImageSize = msg.size();
}

const char* CollabIGTCTImage::getCTImage(){
    const char* ret = this->ctImageMsgBody;
    return ret;
}

int CollabIGTCTImage::getCTImageLength(){
    int ret = this->ctImageSize;
    return ret;
}





