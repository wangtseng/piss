#include "TransferPoint.h"

TransferPoint::TransferPoint()
{

}

TransferPoint::~TransferPoint()
{

}

void TransferPoint::setAbscissaValue(int abscissaValue){
    this->abscissaValue = abscissaValue;
}

void TransferPoint::setOrdinateValue(double ordinateValue){
    this->ordinateValue = ordinateValue;
}

int TransferPoint::getAbscissaValue(){
    return this->abscissaValue;
}

double TransferPoint::getOrdinateValue(){
    return this->ordinateValue;
}
