#ifndef IGTSHOWADVANCEMENTTARGETVELOCITYTASK_H
#define IGTSHOWADVANCEMENTTARGETVELOCITYTASK_H
#include "rcpController.h"
#include <QThread>
//#include "AdvancementSpeedDynamicspline.h"
//#include <QtCharts/QChartView>
#include <QLabel>

class igtShowVelocityIsTask : public QThread
{
    Q_OBJECT
private:
    bool flag;
    int width;
    int height;
    int speed;
    rcpController *controller;
    QLabel *progressSpeedShow;
//    AdvancementSpeedDynamicspline *chart;
//    QChartView *chartView;
public:
    QLabel *getLableView();
    void run();
public:
    igtShowVelocityIsTask(rcpController *controller, int width, int height);
};

#endif // IGTSHOWADVANCEMENTTARGETVELOCITYTASK_H
