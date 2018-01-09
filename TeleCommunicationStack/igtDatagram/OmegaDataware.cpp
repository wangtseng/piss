#include "OmegaDataware.h"


/**
 * @brief igtDataware::igtDataware
 */
OmegaDataware::OmegaDataware()
{

}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::getLatestProgressSpeed
//! \return
//!
void OmegaDataware::getLatestProgressSpeed(double *speed){
    int len = this->omegaPositionSequence.length();
    if(len>2){
        double  current_x = this->omegaPositionSequence.at(len-1)->getPositionX();
        double  current_y = this->omegaPositionSequence.at(len-1)->getPositionY();
        double  current_z = this->omegaPositionSequence.at(len-1)->getPositionZ();

        this->omegaPositionSequence.pop_back();
        double zero_x = this->omegaPositionSequence.at(0)->getPositionX();
        double zero_y = this->omegaPositionSequence.at(0)->getPositionY();
        double zero_z = this->omegaPositionSequence.at(0)->getPositionZ();

        speed[0] = (current_x-zero_x)*1000;
        speed[1] = (current_y-zero_y)*1000;
        speed[2] = (current_z-zero_z)*1000;
    }
    else{
        speed[0] = 0.0;
        speed[1] = 0.0;
        speed[2] = 0.0;
    }
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief igtDataware::clearAllBuffers
//!
void OmegaDataware::clearAllBuffers(){
    this->progressCommandsSequence.clear();
    this->rotateCommandsSequence.clear();
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief igtDataware::getProgressCommandNumber
//! \return
//!
int OmegaDataware::getProgressCommandNumber(){
    int ret;
    progressMutex.lock();
    ret = progressCommandsSequence.size();
    progressMutex.unlock();
    return ret;
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief igtDataware::getTotateCommandNumber
//! \return
//!
int OmegaDataware::getRotateCommandNumber(){
    int ret;
    rotateMutex.lock();
    ret = rotateCommandsSequence.size();
    rotateMutex.unlock();
    return ret;
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief igtDataware::appendProgressCommand
//! \param currentRpmValue
//! \param currentSymbol
//! \param currentMotorType
//!
void OmegaDataware::appendProgressCommand(int currentRpmValue, int currentSymbol, int currentMotorType){

    igtMotorParameter* motorParameter = new igtMotorParameter();
    motorParameter->setRPM(currentRpmValue);
    motorParameter->setSymbol(currentSymbol);
    motorParameter->setMotorType(currentMotorType);

    progressMutex.lock();
    progressCommandsSequence.append(motorParameter);
    progressMutex.unlock();
}

//! -------------------------------------------------------------------------------------------------------
//!
//! \brief igtDataware::appendRotateCommand
//! \param currentRpmValue
//! \param currentSymbol
//! \param currentMotorType
//!
void OmegaDataware::appendRotateCommand(int currentRpmValue, bool currentSymbol, int currentMotorType){
    igtMotorParameter* motorParameter = new igtMotorParameter();
    motorParameter->setRPM(currentRpmValue);
    motorParameter->setSymbol(currentSymbol);
    motorParameter->setMotorType(currentMotorType);

    rotateMutex.lock();
    rotateCommandsSequence.append(motorParameter);
    rotateMutex.unlock();
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::progressCommandsSequenceIsEmpty
//! \return
//!
bool OmegaDataware::progressCommandsSequenceIsEmpty(){
    int ret;
    this->progressMutex.lock();
    ret = this->progressCommandsSequence.size();
    this->progressMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::rotateCommandsSequenceIsEmpty
//! \return
//!
bool OmegaDataware::rotateCommandsSequenceIsEmpty(){
    int ret;
    this->rotateMutex.lock();
    ret = this->rotateCommandsSequence.size();
    this->rotateMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::fetchProgressMotorParameter
//! \return
//!
igtMotorParameter *OmegaDataware::fetchProgressMotorParameter(){
    igtMotorParameter *ret;
    this->progressMutex.lock();
    if(this->progressCommandsSequence.length() == 0){
        return NULL;
    }
    ret = this->progressCommandsSequence[0];
    this->progressMutex.unlock();
    return ret;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::removeFirstProgressMotorParameter
//!
void OmegaDataware::removeFirstProgressMotorParameter(){
    this->progressMutex.lock();
    if(this->progressCommandsSequence.length() == 0){
        return;
    }
    this->progressCommandsSequence.removeFirst();
    this->progressMutex.unlock();
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::fetchRotateMotorParameter
//! \return
//!
igtMotorParameter *OmegaDataware::fetchRotateMotorParameter(){
    igtMotorParameter *ret;
    this->rotateMutex.lock();
    if(this->rotateCommandsSequence.length() == 0){
        return NULL;
    }
    ret = this->rotateCommandsSequence[0];
    this->rotateMutex.unlock();
    return ret;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::removeFirstRotateMotorParameter
//!
void OmegaDataware::removeFirstRotateMotorParameter(){
    this->rotateMutex.lock();
    if(this->rotateCommandsSequence.length() == 0){
        return;
    }
    this->rotateCommandsSequence.removeFirst();
    this->rotateMutex.unlock();
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::appendAdvancementTargetVelocity
//! \param advancementTargetVelocity
//!
void OmegaDataware::appendAdvancementTargetVelocity(igtAdvancementTargetVelocity* advancementTargetVelocity){
    if(advancementTargetVelocity == NULL){
        return;
    }
    this->AdvancementTargetVelocityMutex.lock();
    this->advancementTargetVelocitySequence.append(advancementTargetVelocity);
    this->AdvancementTargetVelocityMutex.unlock();
    return;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::getAdvancementTargetVelocityNumber
//! \return
//!
int OmegaDataware::getAdvancementTargetVelocityNumber(){
    int ret;
    this->AdvancementTargetVelocityMutex.lock();
    ret = this->advancementTargetVelocitySequence.length();
    this->AdvancementTargetVelocityMutex.unlock();
    return ret;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::AdvancementTargetVelocitySequenceIsEmpty
//! \return
//!
bool OmegaDataware::AdvancementTargetVelocitySequenceIsEmpty(){
    int ret;
    this->AdvancementTargetVelocityMutex.lock();
    ret = this->advancementTargetVelocitySequence.size();
    this->AdvancementTargetVelocityMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::fetchAdvancementTargetVelocity
//! \return
//!
igtAdvancementTargetVelocity *OmegaDataware::fetchAdvancementTargetVelocity(){
    igtAdvancementTargetVelocity *advancementTargetVelocity;
    this->AdvancementTargetVelocityMutex.lock();
    if(this->advancementTargetVelocitySequence.length() == 0){
        return NULL;
    }
    advancementTargetVelocity = this->advancementTargetVelocitySequence[0];
    this->AdvancementTargetVelocityMutex.unlock();
    return advancementTargetVelocity;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::removeFirstAdvancementTargetVelocity
//!
void OmegaDataware::removeFirstAdvancementTargetVelocity(){
    this->AdvancementTargetVelocityMutex.lock();
    if(this->advancementTargetVelocitySequence.length() == 0){
        return;
    }
    this->advancementTargetVelocitySequence.removeFirst();
    this->AdvancementTargetVelocityMutex.unlock();
    return;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::setOmegaPosition
//! \param px
//! \param py
//! \param pz
//!
void OmegaDataware::setOmegaPosition(double px, double py, double pz){
//    qDebug()<<"first"<<px<<py<<pz;
    OmegaPosition *omegaPosition = new OmegaPosition(px, py, pz);
    this->omegaPositionSequence.append(omegaPosition);
    return;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::appendForceTorqueValues
//! \param forceTorqueValues
//!
void OmegaDataware::appendForceTorqueValues(igtForceTorqueValues* forceTorqueValues){
    if(forceTorqueValues == NULL){
        return;
    }
    ForceTorqueValuesMutex.lock();
    forceTorqueValuesSequence.append(forceTorqueValues);
    ForceTorqueValuesMutex.unlock();
    return;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::getAForceTorqueNumber
//! \return
//!
int OmegaDataware::getAForceTorqueNumber(){
    int ret;
    ForceTorqueValuesMutex.lock();
    ret = forceTorqueValuesSequence.length();
    ForceTorqueValuesMutex.unlock();
    return ret;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::forceTorqueSequenceIsEmpty
//! \return
//!
bool OmegaDataware::forceTorqueSequenceIsEmpty(){
    bool ret;
    ForceTorqueValuesMutex.lock();
    ret = forceTorqueValuesSequence.length();
    ForceTorqueValuesMutex.unlock();
    if(ret != 0){
        return false;
    }
    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::fetchForceTorqueValues
//! \return
//!
igtForceTorqueValues *OmegaDataware::fetchForceTorqueValues(){
    igtForceTorqueValues *forceTorqueValues;
    ForceTorqueValuesMutex.lock();
    if(this->forceTorqueValuesSequence.length() == 0){
        return NULL;
    }
    forceTorqueValues = forceTorqueValuesSequence[0];
    ForceTorqueValuesMutex.unlock();
    return forceTorqueValues;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief OmegaDataware::removeFirstForceTorqueValues
//!
void OmegaDataware::removeFirstForceTorqueValues(){
    ForceTorqueValuesMutex.lock();
    if(this->forceTorqueSequenceIsEmpty()){
        return;
    }
    forceTorqueValuesSequence.removeFirst();
    ForceTorqueValuesMutex.unlock();
}
