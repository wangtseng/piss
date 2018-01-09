#ifndef OMEGAPOSITION_H
#define OMEGAPOSITION_H


class OmegaPosition
{

public:
    void setPositionX(double px);
    double getPositionX();

    void setPositionY(double py);
    double getPositionY();

    void setPositionZ(double pz);
    double getPositionZ();

private:
    double px;
    double py;
    double pz;

public:
    OmegaPosition(double px, double py, double pz);
};

#endif // OMEGAPOSITION_H
