#include "igtShowForceTorqueValuesTask.h"

igtShowForceTorqueValuesTask::igtShowForceTorqueValuesTask(rcpController * controller)
{
    this->controller = controller;
    this->flag = true;
}

//void igtShowForceTorqueValuesTask::showForceTorqueFeedback(){

//    return;
//}

void igtShowForceTorqueValuesTask::run(){
    while(flag){
        if(this->controller->forceTorqueFeedbackSequenceIsEmpty()){
            continue;
        }

        igtForceTorqueValues *forceTorqueFeedback = this->controller->fetchForceTorqueFeedbackValues();
        this->controller->removeFirstAdvancementTargetVelocity();

        emit updateForceTorqueFeedback(forceTorqueFeedback);
        msleep(100);
    }
}
