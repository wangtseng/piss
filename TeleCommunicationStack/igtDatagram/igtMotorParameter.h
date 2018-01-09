#ifndef MOTORPARAMETER_H
#define MOTORPARAMETER_H
#include "igtDatagram.h"

class igtMotorParameter
{
private:
    int rpm; // motor's rpm value
    int round;
    int symbol; // +/-
    int motorType; // 0: progress, 1: rotate

public:
    int getRPM();
    int getRound();
    int getSymbol();
    int getMotorType();

    void setRPM(int rpm);
    void setRound(int round);
    void setSymbol(int symbol);
    void setMotorType(int motorType);

public:
    igtMotorParameter();
};

#endif // MOTORPARAMETER_H
