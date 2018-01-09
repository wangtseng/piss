#ifndef IGTNDIPOSITION_H
#define IGTNDIPOSITION_H
#include "igtDatagram.h"

class igtNDIPosition
{
private:
    double positionX;
    double positionY;
    double positionZ;

    long timestamps;

public:
    void setPositionX(double positionX);
    double getPositionX();

    void setPositionY(double positionY);
    double getPositionY();

    void setPositionZ(double positionZ);
    double getPositionZ();

    void setTimestamps(long timestamps);
    long getTimestamps();

    void transformIgtdatagramToNDIPosition(igtDatagram* datagram);
public:
    igtNDIPosition();
};

#endif // IGTNDIPOSITION_H
