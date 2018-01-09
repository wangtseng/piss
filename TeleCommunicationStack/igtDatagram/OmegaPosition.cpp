#include "OmegaPosition.h"

OmegaPosition::OmegaPosition(double px, double py, double pz)
{
    this->setPositionX(px);
    this->setPositionY(py);
    this->setPositionZ(pz);
}

void OmegaPosition::setPositionX(double px){
    this->px = px;
}

double OmegaPosition::getPositionX(){
    return px;
}

void OmegaPosition::setPositionY(double py){
    this->py = py;
}

double OmegaPosition::getPositionY(){
    return py;
}

void OmegaPosition::setPositionZ(double pz){
    this->pz = pz;
}

double OmegaPosition::getPositionZ(){
    return pz;
}

