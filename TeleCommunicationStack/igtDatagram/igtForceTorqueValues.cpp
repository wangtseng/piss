#include "igtForceTorqueValues.h"

igtForceTorqueValues::igtForceTorqueValues()
{

}


void igtForceTorqueValues::setForceFeedbackX(double fx){
    this->fx = fx;
}

void igtForceTorqueValues::setForceFeedbackY(double fy){
    this->fy = fy;
}

void igtForceTorqueValues::setForceFeedbackZ(double fz){
    this->fz = fz;
}

void igtForceTorqueValues::setTorqueFeedbackX(double tx){
    this->tx = tx;
}

void igtForceTorqueValues::setTorqueFeedbackY(double ty){
    this->ty = ty;
}

void igtForceTorqueValues::setTorqueFeedbackZ(double tz){
    this->tz = tz;
}

double igtForceTorqueValues::getForceFeedbackX(){
    return this->fx;
}

double igtForceTorqueValues::getForceFeedbackY(){
    return this->fy;
}

double igtForceTorqueValues::getForceFeedbackZ(){
    return this->fz;
}

double igtForceTorqueValues::getTorqueFeedbackX(){
    return this->tx;
}

double igtForceTorqueValues::getTorqueFeedbackY(){
    return this->ty;
}

double igtForceTorqueValues::getTorqueFeedbackZ(){
    return this->tz;
}

void igtForceTorqueValues::transformIgtdatagramToForceFeedback(igtDatagram* datagram){
    QByteArray forceFeedbackByte = datagram->getIgtDatagramBody();
    int forceFeedback[6];
    forceFeedback[0] = (int)((0x000000ff & forceFeedbackByte[0]) + ((0x000000ff & forceFeedbackByte[1]) << 8) + ((0x000000ff & forceFeedbackByte[2]) << 16) + ((0x000000ff & forceFeedbackByte[3]) << 24));
    forceFeedback[1] = (int)((0x000000ff & forceFeedbackByte[4]) + ((0x000000ff & forceFeedbackByte[5]) << 8) + ((0x000000ff & forceFeedbackByte[6]) << 16) + ((0x000000ff & forceFeedbackByte[7]) << 24));
    forceFeedback[2] = (int)((0x000000ff & forceFeedbackByte[8]) + ((0x000000ff & forceFeedbackByte[9]) << 8) + ((0x000000ff & forceFeedbackByte[10]) << 16) + ((0x000000ff & forceFeedbackByte[11]) << 24));
    forceFeedback[3] = (int)((0x000000ff & forceFeedbackByte[12]) + ((0x000000ff & forceFeedbackByte[13]) << 8) + ((0x000000ff & forceFeedbackByte[14]) << 16) + ((0x000000ff & forceFeedbackByte[15]) << 24));
    forceFeedback[4] = (int)((0x000000ff & forceFeedbackByte[16]) + ((0x000000ff & forceFeedbackByte[17]) << 8) + ((0x000000ff & forceFeedbackByte[18]) << 16) + ((0x000000ff & forceFeedbackByte[19]) << 24));
    forceFeedback[5] = (int)((0x000000ff & forceFeedbackByte[20]) + ((0x000000ff & forceFeedbackByte[21]) << 8) + ((0x000000ff & forceFeedbackByte[22]) << 16) + ((0x000000ff & forceFeedbackByte[23]) << 24));

    double forceFeedbackActual[6];
    forceFeedbackActual[0] = forceFeedback[0]/10000.0;
    forceFeedbackActual[1] = forceFeedback[1]/10000.0;
    forceFeedbackActual[2] = forceFeedback[2]/10000.0;
    forceFeedbackActual[3] = forceFeedback[3]/10000.0;
    forceFeedbackActual[4] = forceFeedback[4]/10000.0;
    forceFeedbackActual[5] = forceFeedback[5]/10000.0;

    this->setForceFeedbackX(forceFeedbackActual[0]);
    this->setForceFeedbackY(forceFeedbackActual[1]);
    this->setForceFeedbackZ(forceFeedbackActual[2]);
    this->setTorqueFeedbackX(forceFeedbackActual[3]);
    this->setTorqueFeedbackY(forceFeedbackActual[4]);
    this->setTorqueFeedbackZ(forceFeedbackActual[5]);

    return;
}




