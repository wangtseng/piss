#ifndef IGTDATAWARE_H
#define IGTDATAWARE_H
#include <QVector>
#include <QMutex>
#include <QDebug>
#include "igtForceTorqueValues.h"
#include "igtMotorParameter.h"
#include "igtAdvancementVelocityIs.h"
#include "OmegaPosition.h"

class OmegaDataware
{
private:


    //! input
    QVector<igtForceTorqueValues*> forceTorqueValuesSequence;
    QMutex ForceTorqueValuesMutex;

    QVector<igtAdvancementTargetVelocity*> advancementTargetVelocitySequence;
    QMutex AdvancementTargetVelocityMutex;

    //! output
    QVector<igtMotorParameter*> progressCommandsSequence;
    QMutex progressMutex;

    QVector<igtMotorParameter*> rotateCommandsSequence;
    QMutex rotateMutex;

    QVector<OmegaPosition *> omegaPositionSequence;
    QMutex omegaPositionMutex;

public:
    void appendProgressCommand(int currentRpmValue, int currentSymbol, int currentMotorType);
    void appendRotateCommand(int currentRpmValue, bool currentSymbol, int currentMotorType);
    int getProgressCommandNumber();
    int getRotateCommandNumber();
    bool progressCommandsSequenceIsEmpty();
    bool rotateCommandsSequenceIsEmpty();

    void clearAllBuffers();

    igtMotorParameter *fetchProgressMotorParameter();    
    void removeFirstProgressMotorParameter();

    igtMotorParameter *fetchRotateMotorParameter();
    void removeFirstRotateMotorParameter();

    void appendAdvancementTargetVelocity(igtAdvancementTargetVelocity* advancementTargetVelocity);
    int getAdvancementTargetVelocityNumber();
    bool AdvancementTargetVelocitySequenceIsEmpty();
    igtAdvancementTargetVelocity *fetchAdvancementTargetVelocity();
    void removeFirstAdvancementTargetVelocity();

    void setOmegaPosition(double px, double py, double pz);

    void getLatestProgressSpeed(double *speed);

    void appendForceTorqueValues(igtForceTorqueValues* forceTorqueValues);
    int getAForceTorqueNumber();
    bool forceTorqueSequenceIsEmpty();
    igtForceTorqueValues *fetchForceTorqueValues();
    void removeFirstForceTorqueValues();


public:
    OmegaDataware();
};

#endif // IGTDATAWARE_H
