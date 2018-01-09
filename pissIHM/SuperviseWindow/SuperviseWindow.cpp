#include "SuperviseWindow.h"

SuperviseWindow::SuperviseWindow(int appWidth, int appHeight) : QWidget()
{
    this->appWidth = appWidth;
    this->appHeight = appHeight;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    this->setWindowOpacity(1);
    this->setMouseTracking(true);
    this->setAutoFillBackground(true);

    this->initVariable();
    this->constructIHM();
    this->setConnections();
    this->drawBackground();

}

//!-----------------------------------------------------------------------------------------------
//!
//! \brief SuperviseWindow::initVariable
//!
void SuperviseWindow::initVariable(){

}

//!-----------------------------------------------------------------------------------------------
//!
//! \brief SuperviseWindow::constructIHM
//!
void SuperviseWindow::constructIHM(){
    this->cArmSuperviseWindow = new QLabel();
    this->cArmSuperviseWindow->setFixedSize(appWidth*0.5, appHeight*0.5);
    this->cArmSuperviseWindow->setStyleSheet("background-color:transparent; border:1px solid white;");

    this->pusherMechanismWindow = new QLabel();
    this->pusherMechanismWindow->setFixedSize(appWidth*0.5, appHeight*0.5);
    this->pusherMechanismWindow->setStyleSheet("background-color:transparent; border:1px solid white;");

    this->patientInteractorWindow = new QLabel();
    this->patientInteractorWindow->setFixedSize(appWidth*0.5, appHeight*0.5);
    this->patientInteractorWindow->setStyleSheet("background-color:transparent; border:1px solid white;");

    this->electrocardiogramDisplay = new QLabel();
    this->electrocardiogramDisplay->setFixedSize(appWidth*0.5, appHeight*0.25);
    this->electrocardiogramDisplay->setStyleSheet("background-color:transparent; border:1px solid white;");

    this->vitalSignsWidget = new QWidget();
    this->vitalSignsWidget->setFixedSize(appWidth*0.5, appHeight*0.25);
    this->vitalSignsWidget->setStyleSheet("background-color:transparent; border:1px solid white;");

    this->patientInfoWidget = new QWidget();
    this->patientInfoWidget->setFixedSize(appWidth*0.5, appHeight*0.5);
    this->patientInfoWidget->setStyleSheet("background-color:transparent; border:1px solid white;");
    this->patientInfoLayout = new QVBoxLayout(patientInfoWidget);
    this->patientInfoLayout->addWidget(electrocardiogramDisplay);
    this->patientInfoLayout->addWidget(vitalSignsWidget);
    this->patientInfoLayout->setMargin(0);
    this->patientInfoLayout->setSpacing(0);

    this->mainLayout = new QGridLayout(this);
    this->mainLayout->addWidget(cArmSuperviseWindow,0,0,1,1);
    this->mainLayout->addWidget(pusherMechanismWindow,0,1,1,1);
    this->mainLayout->addWidget(patientInteractorWindow,1,0,1,1);
    this->mainLayout->addWidget(patientInfoWidget,1,1,1,1);
    this->mainLayout->setMargin(0);
    this->mainLayout->setSpacing(0);
}

//!-----------------------------------------------------------------------------------------------
//!
//! \brief SuperviseWindow::setConnections
//!
void SuperviseWindow::setConnections(){

}

//!-----------------------------------------------------------------------------------------------
//!
//! \brief SuperviseWindow::drawBackground
//!
void SuperviseWindow::drawBackground(){
    pixmap = new QPixmap(":/images/background_darkBlue.png");
    QPalette p =  this->palette();

    p.setBrush(QPalette::Background, QBrush(pixmap->scaled(QSize(appWidth, appHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    this->setPalette(p);
    this->setMask(pixmap->mask());
}

