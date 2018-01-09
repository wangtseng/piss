#include "igtGuidewireMovingDistance.h"


/**
 * @brief igtGuidewireMovingDistance::igtGuidewireMovingDistance
 *
 */
igtGuidewireMovingDistance::igtGuidewireMovingDistance()
{

}

void igtGuidewireMovingDistance::setSpeed(int speed){
    this->speed = speed;
}

int igtGuidewireMovingDistance::getSpeed(){
    return this->speed;
}

void igtGuidewireMovingDistance::setValue(int dist){
    this->valeur = dist;
}

int igtGuidewireMovingDistance::getValue(){
    return valeur;
}

void igtGuidewireMovingDistance::setUnit(int unit){
    this->unit = unit;
}

int igtGuidewireMovingDistance::getUnit(){
    return this->unit;
}

void igtGuidewireMovingDistance::transformIgtdatagramToDistance(igtDatagram* datagram){
    QByteArray NDIposition = datagram->getIgtDatagramBody();
    unsigned int positionXSymbol = (uchar)NDIposition[0];
    this->setValue(positionXSymbol);
}
