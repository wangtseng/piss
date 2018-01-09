#include "rcpController.h"

rcpController::rcpController(igtCommunicationStack *communicationStack, OmegaDataware *dataware){
    this->communicationStack = communicationStack;
    this->dataware = dataware;
    this->connect(this->communicationStack, SIGNAL(localIPDetect(QString)), this, SLOT(getSelfIp(QString)));
    this->connect(this->communicationStack, SIGNAL(newConnection(QString)), this, SLOT(connectBack(QString)));
}

void rcpController::getLatestProgressSpeed(double *speed){
    return this->dataware->getLatestProgressSpeed(speed);
}

void rcpController::clearAllBuffers(){
    this->dataware->clearAllBuffers();
}

int rcpController::getIncomingConnectionCount(){
    return communicationStack->getIncomingConnectionCount();
}

void rcpController::setRotateMotorCommand(int currentRpmValue, bool currentSymbol, int currentMotorType){
    if(this->dataware == NULL){
        return;
    }
    this->dataware->appendRotateCommand(currentRpmValue, currentSymbol, currentMotorType);
}

int rcpController::getRotateMotorCommandCount(){
    int ret;
    if(this->dataware == NULL){
        return -99999;
    }
    ret = this->dataware->getRotateCommandNumber();
    return ret;
}

void rcpController::setProgressMotorCommand(int currentPpmValue, int currentSymbol, int currentMotorType){
    if(this->dataware == NULL){
        return;
    }
    this->dataware->appendProgressCommand(currentPpmValue, currentSymbol, currentMotorType);
}

int rcpController::getProgressMotorCommandCount(){
    int ret;
    if(this->dataware == NULL){
        return -99999;
    }
    ret = this->dataware->getProgressCommandNumber();
    return ret;
}

bool rcpController::lauchServer(){
    bool ret;
    if(this->communicationStack == NULL){
        return false;
    }
    ret = this->communicationStack->lauchServer();
    return ret;
}

bool rcpController::stopServer(){
    bool ret;
    if(this->communicationStack == NULL){
        return false;
    }
    ret = this->communicationStack->stopServer();
    return ret;
}

bool rcpController::connectera(QString targetIpLineEdit,QString targetPortLineEdit, bool MB){
    bool ret = true;
    if(this->communicationStack == NULL){
        return false;
    }
    this->communicationStack->connectera(targetIpLineEdit, targetPortLineEdit, MB);
    return ret;
}

bool rcpController::AdvancementTargetVelocitySequenceIsEmpty(){
    bool ret;
    if(this->dataware == NULL){
        return false;
    }
    ret = this->dataware->AdvancementTargetVelocitySequenceIsEmpty();
    return ret;
}

//! -----------------------------------------------------------------------------------------------------
//!
//! \brief rcpController::fetchAdvancementTargetVelocity
//! \return
//!
igtAdvancementTargetVelocity *rcpController::fetchAdvancementTargetVelocity(){
    igtAdvancementTargetVelocity *advancementTargetVelocity;
    if(this->dataware == NULL){
        return NULL;
    }
    advancementTargetVelocity = this->dataware->fetchAdvancementTargetVelocity();
    return advancementTargetVelocity;
}

void rcpController::removeFirstAdvancementTargetVelocity(){
    if(this->dataware == NULL){
        return;
    }
    this->dataware->removeFirstAdvancementTargetVelocity();
}

bool rcpController::forceTorqueFeedbackSequenceIsEmpty(){
    bool ret;
    if(this->dataware == NULL){
        return false;
    }
    ret = this->dataware->forceTorqueSequenceIsEmpty();
    return ret;
}

igtForceTorqueValues *rcpController::fetchForceTorqueFeedbackValues(){
    igtForceTorqueValues *forceTorqueFeedbackValues;
    if(this->dataware == NULL){
        return NULL;
    }
    forceTorqueFeedbackValues = this->dataware->fetchForceTorqueValues();
    return forceTorqueFeedbackValues;
}

void rcpController::removeFirstForceTorqueFeedbackValues(){
    if(this->dataware == NULL){
        return;
    }
    this->dataware->removeFirstForceTorqueValues();
}

void rcpController::getSelfIp(QString IP){
    QString ret = IP;
    emit localIPDetect(ret);
}

void rcpController::connectBack(QString remoteIP){
    QString ret = remoteIP;
//    this->connectera(ret, "10703");
    emit newConnection(ret);
}
