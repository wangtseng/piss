#include "ControlConsoleWindow.h"


/**
 * @brief ControlConsoleWindow::ControlConsoleWindow
 * @param rect
 * @param surgeryTime
 * @param systemDispatcher
 * @param algorithmTestPlatform
 * @param workspaceColor
 */
ControlConsoleWindow::ControlConsoleWindow(QRect rect,
                                           QTime* surgeryTime,
                                           SystemDispatcher* systemDispatcher,
                                           AlgorithmTestPlatform *algorithmTestPlatform,
                                           QString workspaceColor,
                                           QString configuratonFilePath) : QWidget()
{
    this->width = rect.width();
    this->height = rect.height();

    this->surgeryTime = surgeryTime;
    this->systemDispatcher = systemDispatcher;
    this->algorithmTestPlatform = algorithmTestPlatform;
    this->workspaceColor = workspaceColor;
    this->configuratonFilePath = configuratonFilePath;

    this->initVariable();
    this->constructionIHM();
    this->setConnections();
    this->drawBackground();
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::setPatientHandling
//! \param patientHandling
//!
void ControlConsoleWindow::setPatientHandling(Patient *patientHandling){
    this->patientHandling = patientHandling;
    interventionalRobotControl->setPatientHandling(patientHandling);
    //this->systemDispatcher->setPatientHandling(this->patientHandling);
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::initVariable
//!
void ControlConsoleWindow::initVariable(){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    this->caracterStyle = new QFont("Segoe UI", 12, QFont::AnyStyle, true);
    this->clickCounter = 0;
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::displayWindow
//!
void ControlConsoleWindow::displayWindow(){
    this->showFullScreen();
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::constructionIHM
//!
void ControlConsoleWindow::constructionIHM(){

    interventionalRobotControl = new InterventionalRobotControlWidget(this->systemDispatcher, this->width*0.47, this->height*0.95);

    QLabel *spacer = new QLabel();
    spacer->setFixedSize(this->width*0.06, this->height*0.95);

    pissCanvas3D* ballon = new pissCanvas3D(this->width*0.47, this->height*0.95, this->workspaceColor);

    realTimeTrackingWindow = new QFrame();
    realTimeTrackingWindowLayout = new QHBoxLayout(realTimeTrackingWindow);
    realTimeTrackingWindowLayout->addWidget(interventionalRobotControl);
    realTimeTrackingWindowLayout->addWidget(spacer);
    realTimeTrackingWindowLayout->addWidget(ballon);
    realTimeTrackingWindowLayout->setSpacing(0);
    realTimeTrackingWindowLayout->setMargin(0);

    indicationWindow = new QFrame();
    indicationWindow->setFixedHeight(this->height*0.05);

    guidewareTrackingIndication = new QLabel("Guideware's movement");
    guidewareTrackingIndication->setFixedSize(this->width*0.47, this->height*0.05);
    guidewareTrackingIndication->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    guidewareTrackingIndication->setFont(*this->caracterStyle);
    guidewareTrackingIndication->setAlignment(Qt::AlignCenter);

    ballonStentTranckingArea = new QLabel("Ballon/Stent's Movement");
    ballonStentTranckingArea->setFixedSize(this->width*0.47, this->height*0.05);
    ballonStentTranckingArea->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    ballonStentTranckingArea->setFont(*this->caracterStyle);
    ballonStentTranckingArea->setAlignment(Qt::AlignCenter);

    missionAccomplishimentButton = new QPushButton();
    missionAccomplishimentButton->setFixedSize(this->width*0.06, this->height*0.05);
    missionAccomplishimentButton->setStyleSheet("border: 0px solid aliceBlue;border-radius: 0px;background-color: transparent; color: aliceblue");
    missionAccomplishimentButton->setIcon(QIcon(":/images/reset.png"));
    missionAccomplishimentButton->setIconSize(QSize(this->width*0.06, this->height*0.05));
    missionAccomplishimentButton->setFlat(true);

    indicationWindowLayout = new QHBoxLayout(indicationWindow);
    indicationWindowLayout->addWidget(guidewareTrackingIndication);
    indicationWindowLayout->addWidget(missionAccomplishimentButton);
    indicationWindowLayout->addWidget(ballonStentTranckingArea);
    indicationWindowLayout->setSpacing(0);
    indicationWindowLayout->setMargin(0);

    controlConsoleWindowLayout = new QVBoxLayout(this);
    controlConsoleWindowLayout->addWidget(realTimeTrackingWindow);
    controlConsoleWindowLayout->addWidget(indicationWindow);
    controlConsoleWindowLayout->setSpacing(0);
    controlConsoleWindowLayout->setMargin(0);
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::update
//!
void ControlConsoleWindow::update(){
    this->systemDispatcher->launchOmega(patientHandling);
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::onMissionAccomplishimentButtonClicked
//!
void ControlConsoleWindow::onMissionAccomplishimentButtonClicked(){

    //! ask if terminate navigation mode
    if (!(QMessageBox::information(this,tr("Igss Surgery"),tr("Do you really want to terminate the surgery?"),tr("Yes"),tr("No")))){
        this->systemDispatcher->terminateOmega();

        //do something here...

        emit missionAccomplishiment(this->patientHandling->getCTImagePath());
    }
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::setConnections
//!
void ControlConsoleWindow::setConnections(){
    this->connect(missionAccomplishimentButton, SIGNAL(clicked()), this, SLOT(onMissionAccomplishimentButtonClicked()));
}

//! -----------------------------------------------------------------------------------------------------------
//!
//! \brief ControlConsoleWindow::drawBackground
//!
void ControlConsoleWindow::drawBackground(){
    this->setStyleSheet("background:"+this->workspaceColor+";color:darkgray");
}
