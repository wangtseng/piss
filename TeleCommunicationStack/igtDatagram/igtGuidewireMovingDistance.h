#ifndef IGTGUIDEWIREMOVINGDISTANCE_H
#define IGTGUIDEWIREMOVINGDISTANCE_H

#include "igtDatagram.h"


class igtGuidewireMovingDistance
{

private:
    int valeur;
    int unit;
    int speed;

public:
    void setValue(int valeur);
    int getValue();
    void setUnit(int unit);
    int getUnit();
    void setSpeed(int speed);
    int getSpeed();
    void transformIgtdatagramToDistance(igtDatagram* datagram);

public:
    igtGuidewireMovingDistance();
};

#endif // IGTGUIDEWIREMOVINGDISTANCE_H
