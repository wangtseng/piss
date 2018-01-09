#ifndef RCPCONTROLLER_H
#define RCPCONTROLLER_H

#include "igtCommunicationStack.h"
#include "OmegaDataware.h"

class rcpController : public QObject
{
    Q_OBJECT
private:
    igtCommunicationStack *communicationStack;
    OmegaDataware *dataware;

public:
    void setRotateMotorCommand(int currentRpmValue, bool currentSymbol, int currentMotorType);
    int getRotateMotorCommandCount();
    void setProgressMotorCommand(int currentPpmValue, int currentSymbol, int currentMotorType);
    int getProgressMotorCommandCount();
    bool lauchServer();
    bool stopServer();
    bool connectera(QString targetIpLineEdit,QString targetPortLineEdit, bool MB);
    int getIncomingConnectionCount();
    void clearAllBuffers();

    bool AdvancementTargetVelocitySequenceIsEmpty();
    igtAdvancementTargetVelocity *fetchAdvancementTargetVelocity();
    void removeFirstAdvancementTargetVelocity();

    bool forceTorqueFeedbackSequenceIsEmpty();
    igtForceTorqueValues *fetchForceTorqueFeedbackValues();
    void removeFirstForceTorqueFeedbackValues();

    void getLatestProgressSpeed(double *speed);

public slots:
    void getSelfIp(QString IP);
    void connectBack(QString remoteIP);

signals:
    void localIPDetect(QString IP);
    void newConnection(QString remoteIP);
public:
    rcpController(igtCommunicationStack *communicationStack, OmegaDataware *dataware);
};

#endif // RCPCONTROLLER_H
