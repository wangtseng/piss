#include "SurgerySystemWidget.h"


/**
 * @brief SurgerySystemWidget::SurgerySystemWidget
 * @param width
 * @param height
 * @param systemDispatcher
 * @param algorithmTestPlatform
 * @param workspaceColor
 */
SurgerySystemWidget::SurgerySystemWidget(int width,
                                         int height,
                                         SystemDispatcher* systemDispatcher,
                                         AlgorithmTestPlatform* algorithmTestPlatform,
                                         QString workspaceColor){

    this->width = width;
    this->height = height;
    this->systemDispatcher = systemDispatcher;
    this->algorithmTestPlatform = algorithmTestPlatform;
    this->workspaceColor = workspaceColor;

    this->initVariable();
    this->constructIHM();
    this->setConnections();
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief SurgerySystemWidget::initVariable
//!
void SurgerySystemWidget::initVariable(){
    this->setStyleSheet("background:"+ this->workspaceColor);
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief SurgerySystemWidget::constructIHM
//!
void SurgerySystemWidget::constructIHM(){
    startServerButton = new QPushButton();
    startServerButton->setIcon(QIcon(":/images/start.png"));
    startServerButton->setIconSize(QSize(this->width*0.02, this->height*0.03));
    startServerButton->setFixedSize(this->width*0.02, this->height*0.03);
    startServerButton->setFlat(true);

    stopServerButton = new QPushButton();
    stopServerButton->setIcon(QIcon(":/images/stop.png"));
    stopServerButton->setIconSize(QSize(this->width*0.02, this->height*0.03));
    stopServerButton->setFixedSize(this->width*0.02, this->height*0.03);
    stopServerButton->setFlat(true);

    surgerySystemControlBarItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);

    surgerySystemControlBar = new QWidget();
    surgerySystemControlBar->setFixedHeight(this->height*0.03);
    surgerySystemControlBarLayout = new QHBoxLayout(surgerySystemControlBar);
    surgerySystemControlBarLayout->addWidget(startServerButton);
    surgerySystemControlBarLayout->addWidget(stopServerButton);
    surgerySystemControlBarLayout->addItem(surgerySystemControlBarItem);
    surgerySystemControlBarLayout->setSpacing(0);
    surgerySystemControlBarLayout->setMargin(0);

    //! left area to indicate all module's communication states and track real time communication frame
    communicationStatesContainer = new CommunicationStatesContainer(width*0.6, this->height*0.97, this->workspaceColor);

    //! right area
    systemComponentsContainer = new SystemComponentsContainer(width*0.4, this->height*0.97);

    surgerySystemComponents = new QWidget();
    surgerySystemComponents->setStyleSheet("background-color:"+this->workspaceColor+ "; border: 1px solid gainsboro; ");
    surgerySystemComponentsLayout = new QHBoxLayout(surgerySystemComponents);
    surgerySystemComponentsLayout->addWidget(communicationStatesContainer);
    surgerySystemComponentsLayout->addWidget(systemComponentsContainer);
    surgerySystemComponentsLayout->setSpacing(0);
    surgerySystemComponentsLayout->setMargin(0);

    myLayout = new QVBoxLayout(this);
    myLayout->addWidget(surgerySystemControlBar);
    myLayout->addWidget(surgerySystemComponents);
    myLayout->setSpacing(0);
    myLayout->setMargin(0);
}

//! ----------------------------------------------------------------------------------------------
//!
//! \brief SurgerySystemWidget::setConnections
//!
void SurgerySystemWidget::setConnections(){
    this->connect(startServerButton, SIGNAL(clicked()), this, SLOT(onStartServerButtonClicked()));
    this->connect(stopServerButton, SIGNAL(clicked()), this, SLOT(onStopServerButtonClicked()));
    this->connect(this->communicationStatesContainer, SIGNAL(connectedToServerSignal(QString, int)), this, SLOT(testConnectBack(QString, int)));
}

//! ----------------------------------------------------------------------------------------------
//!
//! \brief SurgerySystemWidget::closeSystem
//!
void SurgerySystemWidget::closeSystem(){
    onStopServerButtonClicked();
}

//! ----------------------------------------------------------------------------------------------
//!
//! \brief SurgerySystemWidget::onStopServerButtonClicked
//!
void SurgerySystemWidget::onStopServerButtonClicked(){
    bool ret = this->systemDispatcher->stopPissServer();

    if(ret){
        this->algorithmTestPlatform->setSystemStatus("platform server closed");
    }
    else{
        this->algorithmTestPlatform->setSystemStatus("please don't click the stop button when nothing happened");
    }
}

//! ----------------------------------------------------------------------------------------------
//!
//! \brief SurgerySystemWidget::testConnectBack
//! \param addr
//! \param port
//!
void SurgerySystemWidget::testConnectBack(QString addr, int port){
    qDebug()<<"testConnectBack:";
    this->systemDispatcher->launchConnectBackProcess(true, addr, port);
}

//! ----------------------------------------------------------------------------------------------
//!
//! \brief SurgerySystemWidget::setSelfInfo
//! \param addr
//! \param port
//!
//void SurgerySystemWidget::setSelfInfo(QString addr, int port){
//    myIpAddressLineEdit->setText(addr);
//    myListenPortLineEdit->setText(QString::number(port, 10));
//}

//! ----------------------------------------------------------------------------------------------
//!
//! \brief SurgerySystemWidget::onStartServerButtonClicked
//!
void SurgerySystemWidget::onStartServerButtonClicked(){

    //! fetch ip adress display at the front table...
    //communicationStatesContainer->setDeviceInfo(0, this->systemDispatcher->getIncomingDevices);
//    communicationStatesContainer->setLocalPCInfo(this->systemDispatcher->getMyDeviceInfo()->getIpAddress(),QString::number(this->systemDispatcher->getMyDeviceInfo()->getClientlistenport()));



//    bool ret = this->systemDispatcher->launchCommunicationStackServer();

//    if(ret){
//        this->algorithmTestPlatform->setSystemStatus("platform server started");
//    }
//    else{
//        this->algorithmTestPlatform->setSystemStatus("platform server can't be started");
//    }

    //this->systemDispatcher->launchConnectBackProcess(true, "172.20.14.18", 12630);
}


