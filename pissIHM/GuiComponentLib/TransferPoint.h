#ifndef TRANSFERPOINT_H
#define TRANSFERPOINT_H


class TransferPoint
{
public:
    TransferPoint();
    ~TransferPoint();
    void setAbscissaValue(int abscissaValue);
    void setOrdinateValue(double ordinateValue);
    int getAbscissaValue();
    double getOrdinateValue();

private:
    int abscissaValue = 0;
    double ordinateValue = 0.0;
};

#endif // TRANSFERPOINT_H
