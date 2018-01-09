#include "ColorPoint.h"

ColorPoint::ColorPoint(){
    this->abscissaValue = 0;
    this->redValue = 0.0;
    this->greenValue = 0.0;
    this->blueValue = 0.0;
}

ColorPoint::~ColorPoint(){}

void ColorPoint::setAbscissaValue(int abscissaValue){
    this->abscissaValue = abscissaValue;
}

void ColorPoint::setRedValue(double redValue){
    this->redValue = redValue;
}

void ColorPoint::setGreenValue(double greenValue){
    this->greenValue = greenValue;
}

void ColorPoint::setBlueValue(double blueValue){
    this->blueValue = blueValue;
}

int ColorPoint::getAbscissaValue(){
    return this->abscissaValue;
}

double ColorPoint::getRedValue(){
    return this->redValue;
}

double ColorPoint::getGreenValue(){
    return this->greenValue;
}

double ColorPoint::getBlueValue(){
    return this->blueValue;
}
