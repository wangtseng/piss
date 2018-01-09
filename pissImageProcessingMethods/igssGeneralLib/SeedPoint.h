#ifndef SEEDPOINT_H
#define SEEDPOINT_H


class SeedPoint
{ 
public:
    void setAbscissa(int _abs);
    void setOrdinate(int _ord);
    void setIsometric(int _iso);

    int getAbscissa();
    int getOrdinate();
    int getIsometric();

private:
    int coordinates[3];

public:
    SeedPoint();
};

#endif // SEEDPOINT_H
