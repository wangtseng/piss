#include "igtAdvancementVelocityIs.h"

igtAdvancementTargetVelocity::igtAdvancementTargetVelocity()
{

}

int igtAdvancementTargetVelocity::getTargetVelocity(){
    return targetVelocity;
}

bool igtAdvancementTargetVelocity::getSymbol(){
    return symbol;
}

int igtAdvancementTargetVelocity::getMotorType(){
    return motorType;
}

void igtAdvancementTargetVelocity::setTargetVelocity(int targetVelocity){
    this->targetVelocity = targetVelocity;
}

void igtAdvancementTargetVelocity::setSymbol(bool symbol){
    this->symbol = symbol;
}

void igtAdvancementTargetVelocity::setMotorType(int motorType){
    this->motorType = motorType;
}

void igtAdvancementTargetVelocity::transformIgtdatagramToAdvancementTargetVelocity(igtDatagram* datagram){
    this->setMotorType((uchar)datagram->getIgtDatagramBody()[0]);
    this->setSymbol((uchar)datagram->getIgtDatagramBody()[1]);
    this->setTargetVelocity(((uchar)datagram->getIgtDatagramBody()[2]) + ((uchar)datagram->getIgtDatagramBody()[3]*256));
    return;
}
