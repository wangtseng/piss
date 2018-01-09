#ifndef IGTSHOWFORCETORQUEVALUESTASK_H
#define IGTSHOWFORCETORQUEVALUESTASK_H

#include <QObject>
#include <QThread>
#include "rcpController.h"
#include "igtForceTorqueValues.h"

class igtShowForceTorqueValuesTask :public QThread
{
    Q_OBJECT
private:
    rcpController * controller;
    bool flag;
public:
    void run();
signals:
    void updateForceTorqueFeedback(igtForceTorqueValues * forceTorqueFeedback);
public:
    igtShowForceTorqueValuesTask(rcpController * controller);
};

#endif // IGTSHOWFORCETORQUEVALUESTASK_H
