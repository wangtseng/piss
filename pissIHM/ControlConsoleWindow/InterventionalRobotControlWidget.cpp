#include "InterventionalRobotControlWidget.h"


/**
 * @brief ControlConsoleWindow::ControlConsoleWindow
 * @param rect
 * @param surgeryTime
 * @param systemDispatcher
 * @param algorithmTestPlatform
 * @param workspaceColor
 */
InterventionalRobotControlWidget::InterventionalRobotControlWidget(SystemDispatcher *controller, int w, int h)
{
    this->controller = controller;
    this->width = w;
    this->height = h;

    this->initVariable();
    this->constructionIHM();
    this->setConnections();
    this->drawBackground();

    this->parameterVisualizeTimer->start(100);
    this->omegaEchantionnageTimer->start(400);
    this->parameterProgressVisualizeTimer->start(200);
    this->parameterRotateVisualizeTimer->start(200);

    this->localPCIndicationLabelClicked();
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::initVariable
//!
void InterventionalRobotControlWidget::initVariable(){
    this->rotatingOrientationFlag = false;
    this->fetchOmegaParameter = false;
    this->workspaceColor = "dimgray";

    this->minuteColor = new QColor(216,38,72);
    this->roundColor = new QColor("orange");
    this->forceTorqueColor = new QColor(152,251,152);
    this->forceTorqueRoundColor = new QColor("orange");

    this->caracterStyle = new QFont("Segoe UI", 12, QFont::AnyStyle, true);
    this->isLocalPCIndicationLabelClicked = false;
    this->isRemotePCIndicationLabelClicked = false;

    this->tigtag = 0;

    this->parameterVisualizeTimer = new QTimer();
    this->parameterProgressMotorTimer = new QTimer();
    this->parameterProgressVisualizeTimer = new QTimer();
    this->parameterRotateVisualizeTimer = new QTimer();
    this->parameterRotateMotorTimer = new QTimer();
    this->omegaEchantionnageTimer = new QTimer();
    this->motivateConnectTimer = new QTimer();
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::setConnections
//!
void InterventionalRobotControlWidget::setConnections(){

    this->connect(this->injectButton,                    SIGNAL(clicked()),  this, SLOT(pushContrastMedia()));
    this->connect(this->contrastLineEdit,                SIGNAL(returnPressed()), this, SLOT(pullBackContrastMedia()));
    this->connect(this->stopAllButton,                   SIGNAL(clicked()),  this, SLOT(stopAll()));
    this->connect(this->motorProgressResetButton,        SIGNAL(clicked()),  this, SLOT(motorProgressReset()));
    this->connect(this->advancementControlRight,         SIGNAL(clicked()),  this, SLOT(speedAddition()));
    this->connect(this->advancementControlLeft,          SIGNAL(clicked()),  this, SLOT(speedSubtraction()));
    this->connect(this->advancementControlUp,            SIGNAL(pressed()),  this, SLOT(advancementControlUpPressed()));
    this->connect(this->advancementControlUp,            SIGNAL(released()), this, SLOT(advancementControlUpReleased()));
    this->connect(this->advancementControlDown,          SIGNAL(pressed()),  this, SLOT(advancementControlDownPressed()));
    this->connect(this->advancementControlDown,          SIGNAL(released()), this, SLOT(advancementControlDownReleased()));
    this->connect(this->rotateSpeedAddition,             SIGNAL(clicked()),  this, SLOT(rotatingSpeedAdditionPressed()));
    //this->connect(this->rotateSpeedAddition,             SIGNAL(released()), this, SLOT(rotatingSpeedAdditionReleased()));
    this->connect(this->rotateSpeedSubtraction,          SIGNAL(clicked()),  this, SLOT(rotatingSpeedSubtractionPressed()));
    //this->connect(this->rotateSpeedSubtraction,          SIGNAL(released()), this, SLOT(rotatingSpeedSubtractionReleased()));
    this->connect(this->rotatingLeft,                    SIGNAL(pressed()),  this, SLOT(rotatingLeftPressed()));
    this->connect(this->rotatingLeft,                    SIGNAL(released()), this, SLOT(rotatingLeftReleased()));
    this->connect(this->rotatingRight,                   SIGNAL(clicked()),  this, SLOT(rotatingRightPressed()));
    //this->connect(this->rotatingRight,                   SIGNAL(released()), this, SLOT(rotatingRightReleased()));
    this->connect(this->localPCIndicationLabel,          SIGNAL(clicked()),  this, SLOT(localPCIndicationLabelClicked()));
    this->connect(this->remotePCIndicationLabel,         SIGNAL(clicked()),  this, SLOT(remotePCIndicationLabelClicked()));
    this->connect(this->parameterRotateVisualizeTimer,   SIGNAL(timeout()),  this, SLOT(updateRotateMotorBuffer()));
    this->connect(this->parameterVisualizeTimer,         SIGNAL(timeout()),  this, SLOT(visualizeParameterInContext()));
    this->connect(this->parameterProgressMotorTimer,     SIGNAL(timeout()),  this, SLOT(updateProgressCommandBuffer()));
    this->connect(this->parameterProgressVisualizeTimer, SIGNAL(timeout()),  this, SLOT(updateProgressMotorBuffer()));
    this->connect(this->parameterRotateMotorTimer,       SIGNAL(timeout()),  this, SLOT(updateRotateCommandBuffer()));
    this->connect(this->omegaEchantionnageTimer,         SIGNAL(timeout()),  this, SLOT(omegaSampling()));
    this->connect(this->motivateConnectTimer,            SIGNAL(timeout()),  this, SLOT(connectera()));
    this->connect(this->controller,                      SIGNAL(localIPDetect(QString)), this, SLOT(getSelfIP(QString)));
    this->connect(this->controller,                      SIGNAL(newConnection(QString)), this,  SLOT(connectBack(QString)));
    this->connect(this->moteurSwitching,                 SIGNAL(currentIndexChanged(const QString &)), this, SLOT(controlObjectChanged(const QString &)));

//    this->connect(this->showForceTorqueValuesTask, SIGNAL(updateForceTorqueFeedback(igtForceTorqueValues*)), this, SLOT(showForceTorqueValues(igtForceTorqueValues*)));
}

//!----------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::pullBackContrastMedia
//!
void InterventionalRobotControlWidget::pullBackContrastMedia(){
    this->patientHandling->appendInjectionCommand(500.0, 2.0);
}

//!----------------------------------------------------------------------------------------------------------
//!
//! \brief pushContrastMedia
//!
void InterventionalRobotControlWidget::pushContrastMedia(){
    QString cs = this->contrastLineEdit->text();

    if(cs.contains("ml")){
        QString vs = cs.split("ml")[0];
        double volume = vs.toDouble();

        cs = this->contrastSpeedLineEdit->text();
        vs = cs.split("ml")[0];
        double speed = vs.toDouble();

        this->patientHandling->appendInjectionCommand(volume, speed);
    }
    else if(cs.contains("mm")){
        QString vs = cs.split("mm")[0];
        double volume = vs.toDouble();

        cs = this->contrastSpeedLineEdit->text();
        vs = cs.split("mm")[0];
        double speed = vs.toDouble();

        this->patientHandling->appendInjectionCommand(volume, speed);
    }
    else if(cs.contains("deg")){
        QString vs = cs.split("deg")[0];
        double volume = vs.toDouble();

        cs = this->contrastSpeedLineEdit->text();
        vs = cs.split("deg")[0];
        double speed = vs.toDouble();

        this->patientHandling->appendInjectionCommand(volume, speed);
    }


}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::onMissionAccomplishimentButtonClicked
//!
void InterventionalRobotControlWidget::stopAll(){
    this->patientHandling->appendCloseSessionCommand();
}

//! ----------------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::controlObjectChanged
//! \param obj
//!
void InterventionalRobotControlWidget::controlObjectChanged(const QString & obj){
    this->patientHandling->setTargetObjectHandling(obj);
}

//!------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::setPatientHandling
//! \param patientHandling
//!
void InterventionalRobotControlWidget::setPatientHandling(Patient *patientHandling){
    this->patientHandling = patientHandling;
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::omegaSampling
//!
void InterventionalRobotControlWidget::omegaSampling(){
    double speed[3];
    bool ret = this->patientHandling->getLatestProgressSpeed(speed);

    if(ret){
        if(this->patientHandling->getTargetObjectHandling() == "Guidewire"){
            if(speed[0] < -5){
                this->patientHandling->appendProgressCommand(int(-speed[0]*200.0/50), 1, 1);
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/50)));
            }
            else if(speed[0] > 5){
                this->patientHandling->appendProgressCommand(int(speed[0]*200.0/70), 0, 1);
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/70)));
            }
            else if((speed[0] <= 5)&&(speed[0]>=0)){
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/70)));
                this->patientHandling->appendProgressCommand(0, 1, 1);
            }
            else{
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/50)));
                this->patientHandling->appendProgressCommand(0, 1, 1);
            }

            //! to be modified
            if(speed[1] < -5){
                this->patientHandling->appendRotateCommand(int(-speed[1]*40.0/110), 1, 2);
            }
            else if(speed[1] > 5){
                this->patientHandling->appendRotateCommand(int(speed[1]*40.0/110), 0, 2);
            }
            else if((speed[1] <= 5)&&(speed[1]>=0)){
                this->patientHandling->appendRotateCommand(0, 0, 2);
            }
            else{
                this->patientHandling->appendRotateCommand(0, 0, 2);
            }
            this->rotateMotor->rotateAngle(int(speed[1]*40.0/110));

        }
        else if(this->patientHandling->getTargetObjectHandling() == "Catheter"){
            if(speed[0] < -5){
                this->patientHandling->appendProgressCommand(int(-speed[0]*200.0/50), 1, 0);
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/50)));
            }
            else if(speed[0] > 5){
                this->patientHandling->appendProgressCommand(int(speed[0]*200.0/70), 0, 0);
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/70)));
            }
            else if((speed[0] <= 5)&&(speed[0]>=0)){
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/70)));
                this->patientHandling->appendProgressCommand(0, 1, 0);
            }
            else{
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/50)));
                this->patientHandling->appendProgressCommand(0, 1, 0);
            }
        }
        else if(this->patientHandling->getTargetObjectHandling() == "lodixanol"){
            if(speed[0] < -20){
                this->patientHandling->appendProgressCommand(int(-speed[0]*200.0/50), 1, 3);
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/50)));
            }
            else if(speed[0] > 20){
                this->patientHandling->appendProgressCommand(int(speed[0]*200.0/70), 0, 3);
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/70)));
            }
            else if((speed[0] <= 20)&&(speed[0]>=0)){
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/70)));
                this->patientHandling->appendProgressCommand(0, 1, 3);
            }
            else{
                this->advancementControlShow->setText(QString::number(int(speed[0]*200.0/50)));
                this->patientHandling->appendProgressCommand(0, 1, 3);
            }
        }
    }
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::visualizeParameterInContext
//!
void InterventionalRobotControlWidget::visualizeParameterInContext(){

    localPCControlLabel->setText(QString::number(tigtag)+":"+QString::number(this->controller->getIncomingConnectionCount()));
    tigtag++;

    this->showForceTorqueValues();
    this->showNdiValues();
    this->showGuidewireMovement();
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::updateRotateCommandBuffer
//!
void InterventionalRobotControlWidget::updateRotateMotorBuffer(){
    rotateBufferIndicationBar->setValue(this->patientHandling->getRotateCommandNumber());
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::updateProgressCommandBuffer
//!
void InterventionalRobotControlWidget::updateProgressCommandBuffer(){
    QString ret;
    ret = this->advancementControlShow->text();
    int temp = ret.toInt();
    if(this->symbol == true){
        this->patientHandling->appendProgressCommand(temp, 1, 0);   //rpm symbol motorType
    }else {
        this->patientHandling->appendProgressCommand(temp, 0, 0);
    }
}

//!-------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::updataProgressMotorBuffer
//!
void InterventionalRobotControlWidget::updateProgressMotorBuffer(){
    this->progressBufferIndicationBar->setValue(this->patientHandling->getProgressCommandNumber());
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::localPCIndicationLabelClicked
//!
void InterventionalRobotControlWidget::localPCIndicationLabelClicked(){
    if(this->isLocalPCIndicationLabelClicked == false){
        //! launch server
        bool iResult;
        iResult = this->controller->lauchServer();
        if(iResult == true){
            localPCIndicationLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: green");
        }else {
            qDebug()<<"lauchServer failed";
            return;
        }
    }
    else{
        bool iResult;
        iResult = this->controller->stopServer();
        //! stop server
        if(iResult == true){

            localPCIndicationLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
        }else {
            qDebug()<<"stopServer failed";
            return;
        }
    }

    this->isLocalPCIndicationLabelClicked = !this->isLocalPCIndicationLabelClicked;
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::connectMotivateOrBack
//! \param MB
//!
void InterventionalRobotControlWidget::connectMotivateOrBack(bool MB){
    if(this->isRemotePCIndicationLabelClicked == false){
        //! motivate connect
        bool iResult;
        iResult = this->controller->connectera(this->remoteIpLabel->text(), this->remoteListeningPort->text(), MB);
        if(iResult == true){
            remotePCIndicationLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: green");
        }else {

            qDebug()<<"lauchServer failed";
            return;
        }
    }

    this->isRemotePCIndicationLabelClicked = !this->isRemotePCIndicationLabelClicked;
}


//!------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::remotePCIndicationLabelClicked
//!
void InterventionalRobotControlWidget::remotePCIndicationLabelClicked(){
    this->connectMotivateOrBack(true);
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::connectera
//!
void InterventionalRobotControlWidget::connectera(){

    bool iResult = this->controller->connectera(this->remoteIpLabel->text(), this->remoteListeningPort->text(), true);

    if(iResult){
        qDebug()<<"connected";
        remotePCIndicationLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: green");
        this->motivateConnectTimer->stop();
    }else {
        qDebug()<<"waiting";
        remotePCIndicationLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: red");
    }
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief speedAddition
//!
void InterventionalRobotControlWidget::speedAddition(){
    QString tempSpeed = this->advancementControlShow->text();
    int ret = tempSpeed.toInt();
    ret++;
    tempSpeed = QString::number(ret, 10);
    this->advancementControlShow->setText(tempSpeed);
    return;
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::rotatingRightPressed
//!
void InterventionalRobotControlWidget::rotatingSpeedAdditionPressed(){
    this->rotateMotor->rotateSpeedAddition();
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::rotatingRightReleased
//!
void InterventionalRobotControlWidget::rotatingSpeedAdditionReleased(){
    this->rotateMotor->stop();
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::rotatingLeftPressed
//!
void InterventionalRobotControlWidget::rotatingSpeedSubtractionPressed(){
    this->rotateMotor->rotateSpeedSubtraction();
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::rotatingLeftReleased
//!
void InterventionalRobotControlWidget::rotatingSpeedSubtractionReleased(){
    this->rotateMotor->stop();
}

//!-----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::rotatingLeftPressed
//!
void InterventionalRobotControlWidget::rotatingLeftPressed(){   
    //this->patientHandling->appendRotateCommand(this->rotateMotor->getRotateSpeed(), this->rotateMotor->getRound(), 1, 2);
}

//!-----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::rotatingLeftReleased
//!
void InterventionalRobotControlWidget::rotatingLeftReleased(){
}

//!-----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::rotatingRightPressed
//!
void InterventionalRobotControlWidget::rotatingRightPressed(){
    //this->patientHandling->appendRotateCommand(this->rotateMotor->getRotateSpeed(), this->rotateMotor->getRound(), 0, 2);
}

//!-----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::rotatingRightReleased
//!
void InterventionalRobotControlWidget::rotatingRightReleased(){
}

//!----------------------------------------------------------------------------------------------------------
//!
//! \brief updateRotateCommandBuffer
//!
void InterventionalRobotControlWidget::updateRotateCommandBuffer(){
    int ret;
     ret = rotateMotor->getRotateSpeed();
    if(this->rotatingOrientationFlag == false){
        this->patientHandling->appendRotateCommand(ret, 0, 1);
    }else{
        this->patientHandling->appendRotateCommand(ret, 1, 1);
    }
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::progress
//!
void InterventionalRobotControlWidget::advancementControlUpPressed(){
    this->symbol = true;
    this->parameterProgressMotorTimer->start(100);
    this->parameterProgressVisualizeTimer->start(100);
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::advancementControlUpReleased
//!
void InterventionalRobotControlWidget::advancementControlUpReleased(){
    this->parameterProgressMotorTimer->stop();
    this->patientHandling->appendProgressCommand(0, 1, 0);
    QTimer::singleShot(200, this->parameterProgressVisualizeTimer, SLOT(stop()));
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::back
//!
void InterventionalRobotControlWidget::advancementControlDownPressed(){
    this->symbol = false;
    this->parameterProgressMotorTimer->start(100);
    this->parameterProgressVisualizeTimer->start(100);
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::advancementControlDownReleased
//!
void InterventionalRobotControlWidget::advancementControlDownReleased(){
    this->parameterProgressMotorTimer->stop();
    this->patientHandling->appendProgressCommand(0, 1, 0);
    QTimer::singleShot(200, this->parameterProgressVisualizeTimer, SLOT(stop()));
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::motorProgressReset
//!
void InterventionalRobotControlWidget::motorProgressReset(){
    this->patientHandling->appendRotateCommand(this->rotateMotor->getRotateSpeed(), 1, 4);
}

//!------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::speedSubtraction
//!
void InterventionalRobotControlWidget::speedSubtraction(){
    QString tempSpeed = this->advancementControlShow->text();
    int ret = tempSpeed.toInt();
    ret--;
    tempSpeed = QString::number(ret, 10);
    this->advancementControlShow->setText(tempSpeed);
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::getSelfIP
//! \param IP
//!
void InterventionalRobotControlWidget::getSelfIP(QString IP){
    
    this->localIpLabel->setText(IP);
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::connectBack
//! \param remoteIP
//!
void InterventionalRobotControlWidget::connectBack(QString remoteIP){

    this->remoteIpLabel->setText(remoteIP);
    this->patientHandling->configurerEchantionnageEtat(true);
    this->connectMotivateOrBack(false);
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::showGuidewireMovement
//!
void InterventionalRobotControlWidget::showGuidewireMovement(){
    int ret = this->patientHandling->fetchGuidewireMovingDistance();
    if(ret != 0){
        this->progressBufferIndicationBar->setValue(ret);
        //qDebug()<<ret;
        this->patientHandling->removeGuidewireMovingDistance();
    }
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::showNdiValues
//!
void InterventionalRobotControlWidget::showNdiValues(){
    igtNDIPosition *ret = this->patientHandling->fetchNDIPosition();

    if(ret != NULL){
        this->abscissa_pos->setText(QString::number(ret->getPositionX()/100) + "." + QString::number((int)ret->getPositionX()%100));
        this->ordinate_pos->setText(QString::number(ret->getPositionY()/100) + "." + QString::number((int)ret->getPositionY()%100));
        this->iso_pos->setText(QString::number(ret->getPositionZ()/100) + "." + QString::number((int)ret->getPositionZ()%100));
        this->patientHandling->removeFirstNDIPosition();
    }
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtControlConsoleWindow::showForceTorqueValues
//!
void InterventionalRobotControlWidget::showForceTorqueValues(){

//    qDebug()<<"igtControlConsoleWindow::showForceTorqueValues";
    igtForceTorqueValues *ret = this->patientHandling->fetchForceTorqueValues();

    if(ret != NULL){
        this->fxVisualizeArea->rotateAngle(int(ret->getForceFeedbackX())*10);
        this->fyVisualizeArea->rotateAngle(int(ret->getForceFeedbackY())*10);
        this->fzVisualizeArea->rotateAngle(int(ret->getForceFeedbackZ())*10);
        this->txVsualizeArea->rotateAngle(int(ret->getTorqueFeedbackX())*10);
        this->tyVisualizeArea->rotateAngle(int(ret->getTorqueFeedbackY())*10);
        this->tzVisualizeArea->rotateAngle(int(ret->getTorqueFeedbackZ())*10);
        this->patientHandling->removeFirstForceTorqueValues();
    }
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::displayWindow
//!
void InterventionalRobotControlWidget::displayWindow(){
    this->showFullScreen();
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::constructionIHM
//!
void InterventionalRobotControlWidget:: constructionIHM(){

    constructForceFeedBackVisualizeArea();
    constructInterventionalToolControlArea();
    constructCommunicationArea();

    controlConsoleWindowLayout = new QVBoxLayout(this);
    controlConsoleWindowLayout->addWidget(forceFeedBackTrackingArea);
    controlConsoleWindowLayout->addWidget(realTimeTrackingWindow);
    controlConsoleWindowLayout->addWidget(remoteControlArea);
    controlConsoleWindowLayout->setSpacing(0);
    controlConsoleWindowLayout->setMargin(0);
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::constructForceFeedBackVisualizeArea
//!
void InterventionalRobotControlWidget::constructForceFeedBackVisualizeArea(){
    fxVisualizeArea = new igtForceFeedBackParameterVisualizeArea("fx",this->width*0.38, this->height*0.17, forceTorqueColor,forceTorqueRoundColor);
    fyVisualizeArea = new igtForceFeedBackParameterVisualizeArea("fy",this->width*0.38, this->height*0.17, forceTorqueColor,forceTorqueRoundColor);
    fzVisualizeArea = new igtForceFeedBackParameterVisualizeArea("fz",this->width*0.38, this->height*0.17, forceTorqueColor,forceTorqueRoundColor);
    txVsualizeArea = new igtForceFeedBackParameterVisualizeArea("tx",this->width*0.38, this->height*0.17, forceTorqueColor, forceTorqueRoundColor);
    tyVisualizeArea = new igtForceFeedBackParameterVisualizeArea("ty",this->width*0.38, this->height*0.17, forceTorqueColor, forceTorqueRoundColor);
    tzVisualizeArea = new igtForceFeedBackParameterVisualizeArea("tz",this->width*0.38, this->height*0.17, forceTorqueColor, forceTorqueRoundColor);

    QLabel* hSpacer_2 = new QLabel();
    hSpacer_2->setFixedHeight(this->height*0.22);

    abscissa_pos = new QLabel("0.0");
    abscissa_pos->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    abscissa_pos->setFont(*this->caracterStyle);
    abscissa_pos->setAlignment(Qt::AlignCenter);

    ordinate_pos = new QLabel("0.0");
    ordinate_pos->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    ordinate_pos->setFont(*this->caracterStyle);
    ordinate_pos->setAlignment(Qt::AlignCenter);

    iso_pos = new QLabel("0.0");
    iso_pos->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    iso_pos->setFont(*this->caracterStyle);
    iso_pos->setAlignment(Qt::AlignCenter);

    abscissa_indication_pos = new QLabel("abcscissa:");
    abscissa_indication_pos->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    abscissa_indication_pos->setFont(*this->caracterStyle);
    abscissa_indication_pos->setAlignment(Qt::AlignCenter);

    ordinate_indication_pos = new QLabel("ordinate:");
    ordinate_indication_pos->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    ordinate_indication_pos->setFont(*this->caracterStyle);
    ordinate_indication_pos->setAlignment(Qt::AlignCenter);

    iso_indication_pos = new QLabel("iso:");
    iso_indication_pos->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    iso_indication_pos->setFont(*this->caracterStyle);
    iso_indication_pos->setAlignment(Qt::AlignCenter);

    ndiVisualiseArea = new QGroupBox();
    ndiVisualiseArea->setStyleSheet("QGroupBox{background-color:transparent; color: orange; border: 0px solid orange; border-radius: 0px;margin-top: 1ex;} ");
    ndiVisualiseArea->setTitle("Ndi Position: ");
    ndiVisualiseArea->setFont(*this->caracterStyle);
    ndiVisualiseArea->setFixedWidth(this->width*0.28);
    ndiVisualiseAreaLayout = new QGridLayout(ndiVisualiseArea);
    ndiVisualiseAreaLayout->addWidget(abscissa_indication_pos,0,0);
    ndiVisualiseAreaLayout->addWidget(ordinate_indication_pos,1,0);
    ndiVisualiseAreaLayout->addWidget(iso_indication_pos,2,0);
    ndiVisualiseAreaLayout->addWidget(abscissa_pos,0,1);
    ndiVisualiseAreaLayout->addWidget(ordinate_pos,1,1);
    ndiVisualiseAreaLayout->addWidget(iso_pos,2,1);

    moteurHandlingIndication = new QLabel("Motor Handling:");
    moteurHandlingIndication->setFont(*this->caracterStyle);

    contrastLabel = new QLabel("Value:");
    contrastLabel->setFont(*this->caracterStyle);

    contrastSpeedLabel = new QLabel("Speed Configuration");
    contrastSpeedLabel->setFont(*this->caracterStyle);

    contrastSpeedLineEdit = new QLineEdit("1.5ml/s");
    contrastSpeedLineEdit->setAlignment(Qt::AlignCenter);
    contrastSpeedLineEdit->setStyleSheet("background-color:transparent; color: orange; border: 0px solid orange; border-radius: 0px;margin-top: 1ex;");
    contrastSpeedLineEdit->setFont(*this->caracterStyle);

    backGripperLabel = new QLabel("Current Volume:");
    backGripperLabel->setFont(*this->caracterStyle);

    moteurSwitching = new QComboBox();
    moteurSwitching->setStyleSheet("QComboBox{background-color:transparent; color: orange; border: opx solid orange; border-radius: 0px;margin-top: 1ex;} ");
    moteurSwitching->setFont(*this->caracterStyle);
    moteurSwitching->insertItem(0, QIcon( "caterpillar_head.jpg" ), "Guidewire");
    moteurSwitching->insertItem(1, QIcon( "momor_head.jpg" ), "Catheter");
    moteurSwitching->insertItem(1, QIcon( "momor_head.jpg" ), "lodixanol");

    contrastLineEdit = new QLineEdit("4.5ml");
    contrastLineEdit->setAlignment(Qt::AlignCenter);
    contrastLineEdit->setStyleSheet("background-color:transparent; color: orange; border: 0px solid orange; border-radius: 0px;margin-top: 1ex;");
    contrastLineEdit->setFont(*this->caracterStyle);

    volumeProgressBar = new QProgressBar();
    volumeProgressBar->setOrientation(Qt::Horizontal);
    volumeProgressBar->setAlignment(Qt::AlignRight);
    volumeProgressBar->setStyleSheet("QProgressBar {border: 0px solid aliceBlue; color: red}"
                                        "QProgressBar::chunk {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 yellow, stop: 1 red);}");
    volumeProgressBar->setMinimum(0);
    volumeProgressBar->setMaximum(10);
    volumeProgressBar->setValue(10);

    moteurSwichingArea = new QGroupBox();
    moteurSwichingArea->setStyleSheet("QGroupBox{background-color:transparent; color: orange; border: 0px solid orange; border-radius: 0px;margin-top: 1ex;} ");
    moteurSwichingArea->setTitle("Moteur Switching");
    moteurSwichingArea->setFont(*this->caracterStyle);
    moteurSwichingAreaLayout = new QGridLayout(moteurSwichingArea);
    moteurSwichingAreaLayout->addWidget(moteurHandlingIndication, 0,0);
    moteurSwichingAreaLayout->addWidget(moteurSwitching, 0,1);
    moteurSwichingAreaLayout->addWidget(contrastLabel, 1,0);
    moteurSwichingAreaLayout->addWidget(contrastLineEdit, 1,1);
    moteurSwichingAreaLayout->addWidget(contrastSpeedLabel, 2,0);
    moteurSwichingAreaLayout->addWidget(contrastSpeedLineEdit, 2,1);
    moteurSwichingAreaLayout->addWidget(backGripperLabel, 3,0);
    moteurSwichingAreaLayout->addWidget(volumeProgressBar, 3,1);
    //moteurSwichingAreaLayout->addWidget(, 4,0);
    moteurSwichingAreaLayout->setSpacing(0);
    moteurSwichingAreaLayout->setMargin(0);

    forceFeedBackParametersVisualizeArea = new QLabel();
    forceFeedBackParametersVisualizeArea->setStyleSheet("background-color: transparent; color: aliceblue");
    forceFeedBackParametersVisualizeArea->setFixedHeight(this->height*0.34);

    forceFeedBackParameterVisualizeAreaLayout = new QGridLayout(forceFeedBackParametersVisualizeArea);
    forceFeedBackParameterVisualizeAreaLayout->addWidget(fxVisualizeArea, 0, 0);
    forceFeedBackParameterVisualizeAreaLayout->addWidget(fyVisualizeArea, 0, 1);
    forceFeedBackParameterVisualizeAreaLayout->addWidget(fzVisualizeArea, 0, 2);
    forceFeedBackParameterVisualizeAreaLayout->addWidget(txVsualizeArea, 1, 0);
    forceFeedBackParameterVisualizeAreaLayout->addWidget(tyVisualizeArea, 1, 1);
    forceFeedBackParameterVisualizeAreaLayout->addWidget(tzVisualizeArea, 1, 2);
    forceFeedBackParameterVisualizeAreaLayout->addWidget(ndiVisualiseArea, 0, 3);
    forceFeedBackParameterVisualizeAreaLayout->addWidget(moteurSwichingArea, 1, 3);
    forceFeedBackParameterVisualizeAreaLayout->setSpacing(0);
    forceFeedBackParameterVisualizeAreaLayout->setMargin(0);

    forceFeedBackTrackingIndicationBar = new QLabel("Force/Torque Parameters Visualization");
    forceFeedBackTrackingIndicationBar->setFixedSize(this->width, this->height*0.05);
    forceFeedBackTrackingIndicationBar->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    forceFeedBackTrackingIndicationBar->setFont(*this->caracterStyle);
    forceFeedBackTrackingIndicationBar->setAlignment(Qt::AlignCenter);

    //! force feedback area
    forceFeedBackTrackingArea = new QLabel();
    forceFeedBackTrackingArea->setStyleSheet("background-color: transparent; color: aliceblue");
    forceFeedBackTrackingArea->setFixedHeight(this->height*0.39);
    forceFeedBackTrackingAreaLayout = new QVBoxLayout(forceFeedBackTrackingArea);
    forceFeedBackTrackingAreaLayout->addWidget(forceFeedBackParametersVisualizeArea);
    forceFeedBackTrackingAreaLayout->addWidget(forceFeedBackTrackingIndicationBar);
    forceFeedBackTrackingAreaLayout->setSpacing(0);
    forceFeedBackTrackingAreaLayout->setMargin(0);
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::constructMouvementArea
//!
void InterventionalRobotControlWidget::constructInterventionalToolControlArea(){

    //! motors mouvement control area
    advancementControlUp = new QPushButton();
    advancementControlUp->setFixedSize(this->width*0.13, this->height*0.15);
    advancementControlUp->setIcon(QIcon(":/images/progress.png"));
    advancementControlUp->setIconSize(QSize(this->width*0.13, this->height*0.15));
    advancementControlUp->setStyleSheet("border:0px");
    advancementControlUp->setFlat(true);

    advancementControlDown = new QPushButton();
    advancementControlDown->setFixedSize(this->width*0.13, this->height*0.15);
    advancementControlDown->setIcon(QIcon(":/images/pullback.png"));
    advancementControlDown->setIconSize(QSize(this->width*0.15, this->height*0.15));
    advancementControlDown->setStyleSheet("border:0px");
    advancementControlDown->setFlat(true);

    advancementControlLeft = new QPushButton();
    advancementControlLeft->setFixedSize(this->width*0.13, this->height*0.15);
    advancementControlLeft->setIcon(QIcon(":/images/substraction.png"));
    advancementControlLeft->setIconSize(QSize(this->width*0.1, this->height*0.15));
    advancementControlLeft->setStyleSheet("border:0px");
    advancementControlLeft->setFlat(true);

    advancementControlRight = new QPushButton();
    advancementControlRight->setFixedSize(this->width*0.13, this->height*0.15);
    advancementControlRight->setIcon(QIcon(":/images/plus.png"));
    advancementControlRight->setIconSize(QSize(this->width*0.1, this->height*0.15));
    advancementControlRight->setStyleSheet("border:0px");
    advancementControlRight->setFlat(true);

    advancementControlShow = new QLabel("200");
    advancementControlShow->setFixedSize(this->width*0.15, this->height*0.15);
    advancementControlShow->setAlignment(Qt::AlignCenter);
    advancementControlShow->setFont(QFont("Segoe UI", 22, QFont::DemiBold, false));

    advancementControlArea = new QLabel();
    advancementControlArea->setFixedSize(this->width*0.4, this->height*0.46);
    advancementControlArea->setStyleSheet("background-color: transparent; color: aliceblue");
    advancementControlAreaLayout = new QGridLayout(advancementControlArea);
    advancementControlAreaLayout->addWidget(advancementControlUp, 1, 2);
    advancementControlAreaLayout->addWidget(advancementControlLeft, 2, 1);
    advancementControlAreaLayout->addWidget(advancementControlShow, 2, 2);
    advancementControlAreaLayout->addWidget(advancementControlRight, 2, 3);
    advancementControlAreaLayout->addWidget(advancementControlDown, 3, 2);
    advancementControlAreaLayout->setSpacing(0);
    advancementControlAreaLayout->setMargin(0);

    //! --------------------------------------------------------------------------------
    //! rotate motor control area
    rotateMotor = new igtAnalogMotor("10", minuteColor, roundColor, this->width*0.4, this->height*0.4);
    rotateMotor->setFixedSize(QSize(this->width*0.4, this->height*0.4));

    rotateSpeedAddition = new QPushButton();
    rotateSpeedAddition->setIcon(QIcon(":/images/1495023218_plus.png"));
    rotateSpeedAddition->setIconSize(QSize(this->height*0.18*0.2, this->height*0.18*0.2));
    rotateSpeedAddition->setFixedSize(this->height*0.18*0.2, this->height*0.18*0.2);
    rotateSpeedAddition->setStyleSheet("background-color:transparent; border: 0px 0px 0px 0px");
    rotateSpeedAddition->setFlat(true);

    rotateSpeedSubtraction = new QPushButton();
    rotateSpeedSubtraction->setIcon(QIcon(":/images/1495023466_minus.png"));
    rotateSpeedSubtraction->setIconSize(QSize(this->height*0.18*0.2, this->height*0.18*0.2));
    rotateSpeedSubtraction->setFixedSize(this->height*0.18*0.2, this->height*0.18*0.2);
    rotateSpeedSubtraction->setStyleSheet("background-color: transparent; border: 0px 0px 0px 0px");
    rotateSpeedSubtraction->setFlat(true);

    rotatingLeft = new QPushButton();
    rotatingLeft->setFixedSize(QSize(this->height*0.06, this->height*0.06));
    rotatingLeft->setIcon(QIcon(":/images/rotatingLeft.png"));
    rotatingLeft->setIconSize(QSize(this->height*0.06, this->height*0.06));
    rotatingLeft->setStyleSheet("background-color: transparent; border:0px 0px 0px 0px");
    rotatingLeft->setFlat(true);

    rotatingRight = new QPushButton();
    rotatingRight->setFixedSize(QSize(this->height*0.06, this->height*0.06));
    rotatingRight->setIcon(QIcon(":/images/rotatingRight.png"));
    rotatingRight->setIconSize(QSize(this->height*0.06, this->height*0.06));
    rotatingRight->setStyleSheet("background-color: transparent; border:0px 0px 0px 0px");
    rotatingRight->setFlat(true);

    rotationControlBar = new QLabel();
    rotationControlBar->setFixedHeight(this->height*0.06);
    rotationControlBar->setStyleSheet("background-color: transparent; color: aliceblue");
    rotationControlBarLayout = new QHBoxLayout(rotationControlBar);
    rotationControlBarLayout->addWidget(rotatingLeft);
    rotationControlBarLayout->addWidget(rotateSpeedSubtraction);
    rotationControlBarLayout->addWidget(rotateSpeedAddition);
    rotationControlBarLayout->addWidget(rotatingRight);
    rotationControlBarLayout->setSpacing(0);
    rotationControlBarLayout->setMargin(0);

    rotatingArea = new QLabel();
    rotatingArea->setFixedSize(this->width*0.4, this->height*0.46);
    rotatingArea->setStyleSheet("background-color: transparent; color: aliceblue");
    rotatingAreaLayout = new QVBoxLayout(rotatingArea);
    rotatingAreaLayout->addWidget(rotateMotor);
    rotatingAreaLayout->addWidget(rotationControlBar);
    rotatingAreaLayout->setSpacing(0);
    rotatingAreaLayout->setMargin(0);

    //! indicate progress/back command buffer's length in real time
    progressBufferIndicationBar = new QProgressBar();
    progressBufferIndicationBar->setOrientation(Qt::Vertical);
    progressBufferIndicationBar->setAlignment(Qt::AlignHCenter);
    progressBufferIndicationBar->setFixedWidth(width*0.05);
    progressBufferIndicationBar->setFont(QFont("Segoe UI", 12, QFont::AnyStyle, true));
    progressBufferIndicationBar->setStyleSheet("QProgressBar {border: 0px solid aliceBlue; color: aliceBlue; background-color:transparent}"
                                               "QProgressBar::chunk {background: rgb(216,38,72)}");
    progressBufferIndicationBar->setMinimum(10);
    progressBufferIndicationBar->setMaximum(30);
    progressBufferIndicationBar->setValue(0);

    //! indicate progress/back command buffer's length in real time
    rotateBufferIndicationBar = new QProgressBar();
    rotateBufferIndicationBar->setOrientation(Qt::Vertical);
    rotateBufferIndicationBar->setAlignment(Qt::AlignHCenter);
    rotateBufferIndicationBar->setFixedWidth(width*0.05);
    rotateBufferIndicationBar->setFont(QFont("Segoe UI", 12, QFont::AnyStyle, true));
    rotateBufferIndicationBar->setStyleSheet("QProgressBar {border: 0px solid aliceBlue; color: aliceBlue; background-color:transparent}"
                                             "QProgressBar::chunk {background:rgb(82,182,172)}");
    rotateBufferIndicationBar->setMinimum(0);
    rotateBufferIndicationBar->setMaximum(100);
    rotateBufferIndicationBar->setValue(0);

    interventionalToolControlArea = new QLabel();
    interventionalToolControlArea->setFixedHeight(this->height*0.46);
    interventionalToolControlAreaLayout = new QHBoxLayout(interventionalToolControlArea);
    interventionalToolControlAreaLayout->addWidget(advancementControlArea);
    interventionalToolControlAreaLayout->addWidget(progressBufferIndicationBar);
    interventionalToolControlAreaLayout->addWidget(rotateBufferIndicationBar);
    interventionalToolControlAreaLayout->addWidget(rotatingArea);
    interventionalToolControlAreaLayout->setSpacing(0);
    interventionalToolControlAreaLayout->setMargin(0);

    interventionalToolIndicationBar = new QLabel("Interventional Equipement Control");
    interventionalToolIndicationBar->setFixedSize(this->width, this->height*0.05);
    interventionalToolIndicationBar->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    interventionalToolIndicationBar->setFont(*this->caracterStyle);
    interventionalToolIndicationBar->setAlignment(Qt::AlignCenter);

    realTimeTrackingWindow = new QFrame();
    realTimeTrackingWindow->setFixedHeight(this->height*0.51);
    realTimeTrackingWindowLayout = new QVBoxLayout(realTimeTrackingWindow);
    realTimeTrackingWindowLayout->addWidget(interventionalToolControlArea);
    realTimeTrackingWindowLayout->addWidget(interventionalToolIndicationBar);
    realTimeTrackingWindowLayout->setSpacing(0);
    realTimeTrackingWindowLayout->setMargin(0);
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::constructionCommunicationArea
//!
void InterventionalRobotControlWidget::constructCommunicationArea(){
    remoteControlArea = new QFrame();
    remoteControlArea->setFixedHeight(this->height*0.1);

    remotePCIndicationLabel = new QPushButton("Remote PC:");
    remotePCIndicationLabel->setFixedSize(this->width*0.25, this->height*0.05);
    remotePCIndicationLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    remotePCIndicationLabel->setFont(*this->caracterStyle);

    remoteIpLabel = new QLineEdit("172.20.141.194");
    remoteIpLabel->setFixedSize(this->width*0.3, this->height*0.05);
    remoteIpLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    remoteIpLabel->setFont(*this->caracterStyle);
    remoteIpLabel->setAlignment(Qt::AlignCenter);

    remoteListeningPort = new QLineEdit("10704");
    remoteListeningPort->setFixedSize(this->width*0.1, this->height*0.05);
    remoteListeningPort->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    remoteListeningPort->setFont(*this->caracterStyle);
    remoteListeningPort->setAlignment(Qt::AlignCenter);

    localPCIndicationLabel = new QPushButton("Local PC:");
    localPCIndicationLabel->setFixedSize(this->width*0.25, this->height*0.05);
    localPCIndicationLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    localPCIndicationLabel->setFont(*this->caracterStyle);

    localIpLabel = new QLabel("172.20.141.191");
    localIpLabel->setFixedSize(this->width*0.3, this->height*0.05);
    localIpLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    localIpLabel->setFont(*this->caracterStyle);
    localIpLabel->setAlignment(Qt::AlignCenter);

    localListeningPort = new QLabel("10703");
    localListeningPort->setFixedSize(this->width*0.1, this->height*0.05);
    localListeningPort->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    localListeningPort->setFont(*this->caracterStyle);
    localListeningPort->setAlignment(Qt::AlignCenter);

    localPCControlLabel = new QLabel();
    localPCControlLabel->setFixedSize(this->width*0.29, this->height*0.05);
    localPCControlLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: rgb(251, 95, 33)");
    localPCControlLabel->setFont(*this->caracterStyle);
    localPCControlLabel->setAlignment(Qt::AlignCenter);

    pressureTestLabel = new QLabel();
    pressureTestLabel->setFixedSize(this->width*0.29, this->height*0.05);
    pressureTestLabel->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: rgb(251, 95, 33)");
    pressureTestLabel->setFont(*this->caracterStyle);
    pressureTestLabel->setAlignment(Qt::AlignCenter);


    stopAllButton = new QPushButton();
    stopAllButton->setFixedSize(this->width*0.06, this->height*0.05);
    stopAllButton->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    stopAllButton->setIcon(QIcon(":/images/no_volume.png"));
    stopAllButton->setIconSize(QSize(this->width*0.04, this->height*0.035));
    stopAllButton->setFlat(true);

    motorProgressResetButton = new QPushButton();
    motorProgressResetButton->setStyleSheet("background-color: transparent; border: 0px 0px 0px 0px");
    motorProgressResetButton->setFixedSize(this->width*0.06, this->height*0.05);
    motorProgressResetButton->setIcon(QIcon(":/images/setting.png"));
    motorProgressResetButton->setIconSize(QSize(this->width*0.04, this->height*0.035));
    motorProgressResetButton->setFlat(true);

    injectButton = new QPushButton();
    injectButton->setStyleSheet("background-color: transparent; border: 0px 0px 0px 0px");
    injectButton->setFixedSize(this->width*0.06, this->height*0.05);
    injectButton->setIcon(QIcon(":/images/injection.png"));
    injectButton->setIconSize(QSize(this->width*0.04, this->height*0.035));
    injectButton->setFlat(true);

    indicationWindowLayout = new QGridLayout(remoteControlArea);
    indicationWindowLayout->addWidget(remotePCIndicationLabel, 0, 0);
    indicationWindowLayout->addWidget(remoteIpLabel, 0, 1);
    indicationWindowLayout->addWidget(remoteListeningPort, 0, 2);
    indicationWindowLayout->addWidget(pressureTestLabel, 0, 3);
    indicationWindowLayout->addWidget(localPCIndicationLabel, 1, 0);
    indicationWindowLayout->addWidget(localIpLabel, 1, 1);
    indicationWindowLayout->addWidget(localListeningPort, 1, 2);
    indicationWindowLayout->addWidget(localPCControlLabel, 1, 3);
    indicationWindowLayout->addWidget(motorProgressResetButton, 1, 4);
    indicationWindowLayout->addWidget(stopAllButton, 1, 5);
    indicationWindowLayout->addWidget(injectButton, 1, 6);
    indicationWindowLayout->setSpacing(0);
    indicationWindowLayout->setMargin(0);
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::drawBackground
//!
void InterventionalRobotControlWidget::drawBackground(){
    this->setStyleSheet("background-color:transparent");
//    pixmap = new QPixmap(":/images/background.png");
//    QPalette p =  this->palette();

//    p.setBrush(QPalette::Background,
//               QBrush(pixmap->scaled(QSize(this->width, this->height),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
//    this->setPalette(p);
//    this->setMask(pixmap->mask());

}

