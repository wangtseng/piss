#ifndef COLORPOINT_H
#define COLORPOINT_H


class ColorPoint
{
public:
    ColorPoint();
    ~ColorPoint();
    void setAbscissaValue(int abscissaValue);
    void setRedValue(double redValue);
    void setGreenValue(double greenValue);
    void setBlueValue(double blueValue);
    int getAbscissaValue();
    double getRedValue();
    double getGreenValue();
    double getBlueValue();

private:
    int abscissaValue;
    double redValue;
    double greenValue;
    double blueValue;

};

#endif // COLORPOINT_H
