#include "CurveReformationWindow.h"

CurveReformationWindow::CurveReformationWindow(int x, int y) : QWidget()
{
    this->width = x*0.8;
    this->height = y*0.8;


    this->initVariable();
    this->drawBackground();
    this->constructIHM();
    this->setConnections();
    this->setEnabled(true);
}

CurveReformationWindow::~CurveReformationWindow(){

}


void CurveReformationWindow::display(){
    this->resize(this->width, this->height);
    this->show();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief CurveReformationWindow::setPatientHandling
//! \param patientHandling
//!
void CurveReformationWindow::setPatientHandling(Patient *patientHandling){
    this->patientHandling = patientHandling;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief CurveReformationWindow::drawBackground
//!
void CurveReformationWindow::drawBackground(){
    pixmap = new QPixmap(":/images/background.png");
    QPalette p =  this->palette();

    p.setBrush(QPalette::Background, QBrush(pixmap->scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    this->setPalette(p);
    this->setMask(pixmap->mask());
}

//!
//! \brief CurveReformationWindow::initVariable
//!
void CurveReformationWindow::initVariable(){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    this->setWindowOpacity(1);
    this->setMouseTracking(true);
    this->setAutoFillBackground(true);
}

//!------------------------------------------------------------------------
//!
//! \brief CurveReformationWindow::constructIHM
//!
void CurveReformationWindow::constructIHM(){

    this->referenceTableWidget = new QWidget();
    this->referenceTableWidget->setFixedSize(width*0.2,height*0.5);
    this->referenceTableWidget->setStyleSheet("background-color:lightCyan");
    this->curvereformationWidget = new QWidget();
    this->curvereformationWidget->setFixedSize(width*0.2,height*0.05);
    this->curvereformationWidget->setStyleSheet("background-color:light");
    this->curvereformationLabel = new QLabel(curvereformationWidget);
    this->curvereformationLabel->setText("curve reformation");
    this->curvereformationLabel->setAlignment(Qt::AlignCenter );
    this->curvereformationLabel->setStyleSheet("QLabel{color:blue£»background:yellow};");

    this->centerlinetreeTreeWidget = new QTreeWidget();
    this->centerlinetreeTreeWidget->setFixedSize(width*0.2,height*0.45);
    this->centerlinetreeTreeWidget->setStyleSheet("background-color:skyblue");
    this->centerlinetreeTreeWidget->setColumnCount(1);
    this->centerlinetreeTreeWidget->setHeaderLabel(tr("Centerline Choose"));


    this->referenceTableLayout = new QVBoxLayout(referenceTableWidget);
    this->referenceTableLayout->addWidget(curvereformationWidget);
    this->referenceTableLayout->addWidget(centerlinetreeTreeWidget);
    this->referenceTableLayout->setSpacing(0);
    this->referenceTableLayout->setMargin(0);


    this->manipulationWidget = new QWidget();
    this->manipulationWidget->setFixedSize(width*0.2,height*0.5);
    this->manipulationWidget->setStyleSheet("background-color:aliceblue");
    manipulationItemWidget = new QWidget();
    manipulationItemLayout = new QVBoxLayout(manipulationItemWidget);
    fermePushBtn = new QPushButton(manipulationItemWidget);
    fermePushBtn->setText("ferme");
    fermePushBtn->setGeometry(0,0,manipulationWidget->width()*0.3,manipulationWidget->height()*0.1);
    resetPushBtn = new QPushButton(manipulationItemWidget);
    resetPushBtn->setText("reset");
    resetPushBtn->setGeometry(0,manipulationWidget->height()*0.4,manipulationWidget->width()*0.3,manipulationWidget->height()*0.1);
    commencePushBtn= new QPushButton(manipulationItemWidget);
    commencePushBtn->setText("start");
    commencePushBtn->setGeometry(0,manipulationWidget->height()*0.85,manipulationWidget->width()*0.3,manipulationWidget->height()*0.1);
    manipulationindeterminateWidget = new QWidget();
    this->manipulationLayout = new QHBoxLayout(manipulationWidget);
    this->manipulationLayout->addWidget(manipulationItemWidget);
    this->manipulationLayout->addWidget(manipulationindeterminateWidget);


    this->volumeWidget = new QWidget();
    this->volumeWidget->setFixedSize(width*0.4,height*0.5);
    this->volumeWidget->setStyleSheet("background-color:orange");
    volumeVTKWidget = new QVTKWidget();
    this->volumeWidgetLayout =new QVBoxLayout(volumeWidget);
    this->volumeWidgetLayout->addWidget(volumeVTKWidget);

    this->centerLineWidget = new QWidget();
    this->centerLineWidget->setFixedSize(width*0.4,height*0.5);
    this->centerLineWidget->setStyleSheet("background-color:yellow");
    centerLineVTKWidget = new QVTKWidget();
    this->centerLineWidgetLayout =new QVBoxLayout(centerLineWidget);
    this->centerLineWidgetLayout->addWidget(centerLineVTKWidget);

    this->volumeAndCenterLineMergeWidget = new QWidget();
    this->volumeAndCenterLineMergeWidget->setFixedSize(width*0.4,height*0.5);
    this->volumeAndCenterLineMergeWidget->setStyleSheet("background-color:blue");
    volumeAndCenterLineMergeVTKWidget = new QVTKWidget();
    this->volumeAndCenterLineMergeWidgetLayout =new QVBoxLayout(volumeAndCenterLineMergeWidget);
    this->volumeAndCenterLineMergeWidgetLayout->addWidget(volumeAndCenterLineMergeVTKWidget);

    this->reformatedWindow = new QWidget();
    this->reformatedWindow->setFixedSize(width*0.4,height*0.5);
    this->reformatedWindow->setStyleSheet("background-color:green");
    this->reconstructionVolumeVTKWidget = new QVTKWidget();
    this->reconstructionVolumeVTKWidget->setFixedSize(width*0.2,height*0.5);
    this->reconstructionVolumeVTKWidget->setStyleSheet("background-color:skyblue");
    this->cprvtkWidget= new QVTKWidget();
    this->cprvtkWidget->setFixedSize(width*0.2,height*0.5);
    this->cprvtkWidget->setStyleSheet("background-color:skyblue");
    this->reformatedWindowLayout = new QHBoxLayout(reformatedWindow);
    this->reformatedWindowLayout->addWidget(reconstructionVolumeVTKWidget);
    this->reformatedWindowLayout->addWidget(cprvtkWidget);
//    this->reformatedWindowLayout->setSpacing(0);
//    this->reformatedWindowLayout->setMargin(0);

    this->myLayout = new QGridLayout(this);
    this->myLayout->addWidget(referenceTableWidget,0,0);
    this->myLayout->addWidget(manipulationWidget,1,0);
    this->myLayout->addWidget(volumeWidget,0,1);
    this->myLayout->addWidget(centerLineWidget,0,2);
    this->myLayout->addWidget(volumeAndCenterLineMergeWidget,1,1);
    this->myLayout->addWidget(reformatedWindow,1,2);
    this->myLayout->setSpacing(0);
    this->myLayout->setMargin(0);
}

//!------------------------------------------------------------------------
//!
//! \brief CurveReformationWindow::setConnections
//!
void CurveReformationWindow::setConnections(){

    this->connect(fermePushBtn, SIGNAL(clicked()), this, SLOT(ferme()));
    this->connect(resetPushBtn, SIGNAL(clicked()), this, SLOT(reset()));
    this->connect(commencePushBtn, SIGNAL(clicked()), this, SLOT(commence()));
}

//!------------------------------------------------------------------------
//!
//! \brief CurveReformationWindow::ferme
//!
void CurveReformationWindow::ferme(){
    this->close();
}


#include"SurgeryPlanWindow.h"
//!
//! \brief CurveReformationWindow::reset
//!
void CurveReformationWindow::reset(){

}

//!
//! \brief CurveReformationWindow::commence
//!
void CurveReformationWindow::commence(){

}


