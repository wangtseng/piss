#ifndef ADVANCMENTTARGETVELOCITY_H
#define ADVANCMENTTARGETVELOCITY_H
#include "igtDatagram.h"

class igtAdvancementTargetVelocity
{
private:
    int targetVelocity; // motor's rpm value
    bool symbol; // +/-
    int motorType; // 0: progress, 1: rotate
public:
    int getTargetVelocity();
    bool getSymbol();
    int getMotorType();

    void setTargetVelocity(int targetVelocity);
    void setSymbol(bool symbol);
    void setMotorType(int motorType);

    void transformIgtdatagramToAdvancementTargetVelocity(igtDatagram* datagram);

public:
    igtAdvancementTargetVelocity();
};

#endif // ADVANCMENTTARGETVELOCITY_H
