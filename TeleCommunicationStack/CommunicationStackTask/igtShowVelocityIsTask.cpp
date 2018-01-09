#include "igtShowVelocityIsTask.h"

igtShowVelocityIsTask::igtShowVelocityIsTask(rcpController *controller, int width, int height)
{
    this->speed = 0;
    this->flag = true;
    this->controller = controller;
    this->width = width;
    this->height = height;
//    this->chart = new AdvancementSpeedDynamicspline();
//    this->chartView = new QChartView(chart);
//    chartView->setFixedHeight(this->height*0.2);
//    chartView->setRenderHint(QPainter::Antialiasing);
    this->progressSpeedShow = new QLabel("0");
    this->progressSpeedShow->setStyleSheet("background-color: transparent; border: 0px 0px 0px 0px; color: white");
    this->progressSpeedShow->setFixedSize(this->height*0.04, this->height*0.04);
}

void igtShowVelocityIsTask::run(){
    while(flag){
        if(this->controller->AdvancementTargetVelocitySequenceIsEmpty()){
            continue;
        }
        igtAdvancementTargetVelocity *advancementTargetVelocity = this->controller->fetchAdvancementTargetVelocity();
        if(advancementTargetVelocity->getSymbol() == 0){
            speed = advancementTargetVelocity->getTargetVelocity();
        }else {
            speed = - advancementTargetVelocity->getTargetVelocity();
        }
        this->progressSpeedShow->setText(QString::number(speed));
        this->controller->removeFirstAdvancementTargetVelocity();
        msleep(100);
    }
}

QLabel *igtShowVelocityIsTask::getLableView(){
    QLabel *ret;
    if(this->progressSpeedShow == NULL){
        return NULL;
    }
    ret = this->progressSpeedShow;
    return ret;
}

