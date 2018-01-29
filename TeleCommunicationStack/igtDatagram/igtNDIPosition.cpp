#include "igtNDIPosition.h"
#include <QDebug>

igtNDIPosition::igtNDIPosition()
{

}

void igtNDIPosition::setTimestamps(long timestamps){
    this->timestamps = timestamps;
}

long igtNDIPosition::getTimestamps(){
    return this->timestamps;
}

void igtNDIPosition::setPositionX(double positionX){
    this->positionX = positionX;
}

double igtNDIPosition::getPositionX(){
    return this->positionX;
}

void igtNDIPosition::setPositionY(double positionY){
    this->positionY = positionY;
}

double igtNDIPosition::getPositionY(){
    return this->positionY;
}

void igtNDIPosition::setPositionZ(double positionZ){
    this->positionZ = positionZ;
}

double igtNDIPosition::getPositionZ(){
    return this->positionZ;
}

void igtNDIPosition::transformIgtdatagramToNDIPosition(igtDatagram* datagram){
    QByteArray NDIposition = datagram->getIgtDatagramBody();

    this->setTimestamps(datagram->getTimeStampes());

    unsigned int positionXSymbol = (unsigned int)NDIposition[0];
    int temp1 = (unsigned int)((0x000000ff & NDIposition[1]) + ((0x000000ff & NDIposition[2])<<8));

    unsigned int positionYSymbol = (unsigned int)NDIposition[3];
    int temp2 = (unsigned int)((0x000000ff & NDIposition[4]) + ((0x000000ff & NDIposition[5])<<8));

    unsigned int positionZSymbol = (unsigned int)NDIposition[6];
    int temp3 = (unsigned int)((0x000000ff & NDIposition[7]) + ((0x000000ff & NDIposition[8])<<8));

    if(positionXSymbol != 0){
        temp1 = -temp1;
    }
    if(positionYSymbol != 0){
        temp2 = -temp2;
    }
    if(positionZSymbol != 0){
        temp3 = -temp3;
    }

    double abssisa = temp1*1.0/100;
    double ordinate = temp2*1.0/100;
    double iso = temp3*1.0/100;

    this->setPositionX(abssisa);
    this->setPositionY(ordinate);
    this->setPositionZ(iso);

    //qDebug()<<"position recu:  "<<abssisa<<ordinate<<iso;
}
