#include "igtMotorParameter.h"

igtMotorParameter::igtMotorParameter(){
    this->round = 0;
}

int igtMotorParameter::getRPM(){
    return rpm;
}

int igtMotorParameter::getRound(){
    return round;
}

int igtMotorParameter::getSymbol(){
    return symbol;
}

int igtMotorParameter::getMotorType(){
    return motorType;
}

void igtMotorParameter::setRPM(int rpm){
    this->rpm = rpm;
}

void igtMotorParameter::setSymbol(int symbol){
    this->symbol = symbol;
}

void igtMotorParameter::setMotorType(int motorType){
    this->motorType = motorType;
}

void igtMotorParameter::setRound(int round){
    this->round = round;
}
