#ifndef FORCETORQUEVALUES_H
#define FORCETORQUEVALUES_H
#include "igtDatagram.h"

class igtForceTorqueValues
{

private:
    double fx;
    double fy;
    double fz;
    double tx;
    double ty;
    double tz;
public:
    void setForceFeedbackX(double fx);
    void setForceFeedbackY(double fy);
    void setForceFeedbackZ(double fz);
    void setTorqueFeedbackX(double tx);
    void setTorqueFeedbackY(double ty);
    void setTorqueFeedbackZ(double tz);

    double getForceFeedbackX();
    double getForceFeedbackY();
    double getForceFeedbackZ();
    double getTorqueFeedbackX();
    double getTorqueFeedbackY();
    double getTorqueFeedbackZ();

    void transformIgtdatagramToForceFeedback(igtDatagram* datagram);

public:
    igtForceTorqueValues();
};

#endif // FORCETORQUEVALUES_H


