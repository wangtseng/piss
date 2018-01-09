#ifndef IGTCONTRASTMEDIAPUSH_H
#define IGTCONTRASTMEDIAPUSH_H


class igtContrastMediaPush
{

public:
    void setSpeed(double speed);
    void setVolume(double volume);

    double getSpeed();
    double getVolume();

private:
    double speed;
    double volume;

public:
    igtContrastMediaPush();
};

#endif // IGTCONTRASTMEDIAPUSH_H
