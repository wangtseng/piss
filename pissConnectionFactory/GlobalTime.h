#ifndef GLOBALTIME_H
#define GLOBALTIME_H

#define     MAX_PACKET_SIZE     1024

class GlobalTime
{
public:
    long currentTime();

private:
    long m_frequency;

public:
    GlobalTime();
};

#endif // GLOBALTIME_H
