#ifndef HELLOMESSAGE_H
#define HELLOMESSAGE_H

#include "igtDatagram.h"

class igtHelloMessage : public igtDatagram
{
private:
    long count;
    int connectionState;
public:
    void setCount(long count);
    long getCount();

    void setConnectionState(int connectionState);
    int getConnectionState();

    igtDatagram *transformHelloMessageToIgtDatagram();
public:
    igtHelloMessage();
};

#endif // HELLOMESSAGE_H
