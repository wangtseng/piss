#include "CommunicationStatesContainer.h"



/**
 * @brief CommunicationStatesContainer::CommunicationStatesContainer
 * @param width
 * @param height
 */
CommunicationStatesContainer::CommunicationStatesContainer(int width, int height, QString workspaceColor): QFrame(){

    this->width = width;
    this->height = height;
    this->workspaceColor = workspaceColor;

    this->setFixedSize(this->width, this->height);

    this->initVariable();
    this->constructIHM();
    this->setConnections();
    this->embellshingIHM();
}

//! ---------------------------------------------------------------------------------------------------
//!
//! \brief CommunicationStatesContainer::setLocalPCInfo
//! \param addr
//! \param port
//!
void CommunicationStatesContainer::setLocalPCInfo(QString addr, QString port){
    myIpAddressLineEdit->setText(addr);
    myListenPortLineEdit->setText(port);
}



//! ---------------------------------------------------------------------------------------------------
//!
//! \brief CommunicationStatesContainer::initVariable
//!
void CommunicationStatesContainer::initVariable(){

    hardwareHeader<<"device"<<"address"<<"port"<<"transmission"<<"reception"<<"control";

    transmissionHeader<<"channel"<<"version"<<"type"<<"device"<<"timestamp"<<"dlc"<<"data0"<<"data1"<<"data2"<<"data3"<<"data4"<<"data5"<<"data6"<<"data7"<<"data8"<<"data9"<<"...";

    groupBoxStyleSheet = "QGroupBox {background-color: transparent; border: 0px solid beige; margin-top: 1ex;color:lightgray}"
                         "QGroupBox::title {subcontrol-origin: margin;subcontrol-position: bottom right; padding: 0 3px;color: lightgray}";
}

//! ---------------------------------------------------------------------------------------------------
//!
//! \brief CommunicationStatesContainer::constructIHM slategray
//!
void CommunicationStatesContainer::constructIHM(){

    myInfoIndication = new QLabel("Local PC Info:");
    myInfoIndication->setStyleSheet("background-color:tomato; color:"+workspaceColor+"; border:0px solid gainsboro");
    myInfoIndication->setAlignment(Qt::AlignCenter);
    myInfoIndication->setFont(QFont("Times", 8, QFont::AnyStyle, false));
    myInfoIndication->setFixedSize(this->width*0.07, this->height*0.03);

    myIpAddress = new QLabel("ip:");
    myIpAddress->setStyleSheet("background-color:transparent; color:tomato; border-width:0px 0px 0px 0px");
    myIpAddress->setAlignment(Qt::AlignCenter);
    myIpAddress->setFont(QFont("Times", 8, QFont::AnyStyle, false));
    myIpAddress->setFixedSize(this->width*0.04, this->height*0.03);

    myIpAddressLineEdit = new QLineEdit();
    myIpAddressLineEdit->setStyleSheet("background-color:transparent; color:tomato; border-width:0px 0px 1px 0px; border-color:lightgray");
    myIpAddressLineEdit->setAlignment(Qt::AlignCenter);
    myIpAddressLineEdit->setFont(QFont("Times", 8, QFont::AnyStyle, false));
    myIpAddressLineEdit->setFixedSize(this->width*0.09, this->height*0.03);

    myListenPort = new QLabel("port:");
    myListenPort->setStyleSheet("background-color:transparent; color:tomato; border-width:0px 0px 0px 0px");
    myListenPort->setAlignment(Qt::AlignCenter);
    myListenPort->setFont(QFont("Times", 8, QFont::AnyStyle, false));
    myListenPort->setFixedSize(this->width*0.04, this->height*0.03);

    myListenPortLineEdit = new QLineEdit();
    myListenPortLineEdit->setStyleSheet("background-color:transparent; color:tomato; border-width:0px 0px 1px 0px; border-color:lightgray");
    myListenPortLineEdit->setAlignment(Qt::AlignCenter);
    myListenPortLineEdit->setFont(QFont("Times", 8, QFont::AnyStyle, false));
    myListenPortLineEdit->setFixedSize(this->width*0.05, this->height*0.03);

    selfInfoAreaItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->selfInfoArea = new QLabel();
    this->selfInfoArea->setStyleSheet("background:transparent; border:0px solid gray");
    this->selfInfoArea->setFixedSize(0.5*width, 0.03*height);
    this->selfInfoAreaLayout = new QHBoxLayout(this->selfInfoArea);
    this->selfInfoAreaLayout->addWidget(myInfoIndication);
    this->selfInfoAreaLayout->addWidget(myIpAddress);
    this->selfInfoAreaLayout->addWidget(myIpAddressLineEdit);
    this->selfInfoAreaLayout->addWidget(myListenPort);
    this->selfInfoAreaLayout->addWidget(myListenPortLineEdit);
    this->selfInfoAreaLayout->addItem(selfInfoAreaItem);
    this->selfInfoAreaLayout->setSpacing(0);
    this->selfInfoAreaLayout->setMargin(0);


    targetInfoIndication = new QLabel("Target PC:");
    targetInfoIndication->setStyleSheet("background-color:springgreen; color:"+workspaceColor+"; border:0px solid gainsboro");
    targetInfoIndication->setAlignment(Qt::AlignCenter);
    targetInfoIndication->setFont(QFont("Times", 8, QFont::AnyStyle, false));
    targetInfoIndication->setFixedSize(this->width*0.07, this->height*0.03);

    targetIpAddress = new QLabel("ip:");
    targetIpAddress->setStyleSheet("background-color:transparent; color:springgreen; border-width:0px 0px 0px 0px");
    targetIpAddress->setAlignment(Qt::AlignCenter);
    targetIpAddress->setFont(QFont("Times", 8, QFont::AnyStyle, false));
    targetIpAddress->setFixedSize(this->width*0.04, this->height*0.03);

    targetIpAddressLineEdit = new QLineEdit();
    targetIpAddressLineEdit->setText("172.20.141.229");
    targetIpAddressLineEdit->setStyleSheet("background-color:transparent; color:springgreen; border-width:0px 0px 1px 0px; border-color:lightgray");
    targetIpAddressLineEdit->setAlignment(Qt::AlignCenter);
    targetIpAddressLineEdit->setFont(QFont("Times", 8, QFont::AnyStyle, false));
    targetIpAddressLineEdit->setFixedSize(this->width*0.09, this->height*0.03);

    targetListenPort = new QLabel("port:");
    targetListenPort->setStyleSheet("background-color:transparent; color:springgreen; border-width:0px 0px 0px 0px");
    targetListenPort->setAlignment(Qt::AlignCenter);
    targetListenPort->setFont(QFont("Times", 8, QFont::AnyStyle, false));
    targetListenPort->setFixedSize(this->width*0.04, this->height*0.03);

    targetListenPortLineEdit = new QLineEdit();
    targetListenPortLineEdit->setText("10703");
    targetListenPortLineEdit->setStyleSheet("background-color:transparent; color:springgreen; border-width:0px 0px 1px 0px; border-color:lightgray");
    targetListenPortLineEdit->setAlignment(Qt::AlignCenter);
    targetListenPortLineEdit->setFont(QFont("Times", 8, QFont::AnyStyle, false));
    targetListenPortLineEdit->setFixedSize(this->width*0.05, this->height*0.03);


    testConnectBackButton = new QPushButton();
    testConnectBackButton->setIcon(QIcon(":/images/title.png"));
    testConnectBackButton->setIconSize(QSize(this->width*0.02, this->height*0.03));
    testConnectBackButton->setFixedSize(this->width*0.02, this->height*0.03);
    testConnectBackButton->setFlat(true);

    this->tagetTestArea = new QLabel();
    this->tagetTestArea->setStyleSheet("background:transparent; border:0px solid gray");
    this->tagetTestArea->setFixedSize(0.5*width, 0.03*height);
    this->targetTestAreaLayout = new QHBoxLayout(this->tagetTestArea);
    this->targetTestAreaLayout->addWidget(targetInfoIndication);
    this->targetTestAreaLayout->addWidget(targetIpAddress);
    this->targetTestAreaLayout->addWidget(targetIpAddressLineEdit);
    this->targetTestAreaLayout->addWidget(targetListenPort);
    this->targetTestAreaLayout->addWidget(targetListenPortLineEdit);
    this->targetTestAreaLayout->addWidget(testConnectBackButton);
    this->targetTestAreaLayout->addItem(selfInfoAreaItem);
    this->targetTestAreaLayout->setSpacing(0);
    this->targetTestAreaLayout->setMargin(0);

    //!

    this->HardwareInformation = new QTableWidget(5, 6);
    this->HardwareInformation->setFixedSize(0.5*width, 0.24*height);
    this->HardwareInformation->setHorizontalHeaderLabels(hardwareHeader);
    this->HardwareInformation->verticalHeader()->setVisible(false);
    this->HardwareInformation->verticalScrollBar()->setVisible(false);
    this->HardwareInformation->horizontalScrollBar()->setVisible(false);
    this->HardwareInformation->setColumnWidth(0, 0.06*width);
    this->HardwareInformation->setColumnWidth(1, 0.08*width);
    this->HardwareInformation->setColumnWidth(2, 0.08*width);
    this->HardwareInformation->setColumnWidth(3, 0.08*width);
    this->HardwareInformation->setColumnWidth(4, 0.08*width);
    this->HardwareInformation->setColumnWidth(5, 0.06*width);

    this->HardwareInformationArea = new QLabel();
    this->HardwareInformationArea->setStyleSheet("border:0px solid lightgray;background:"+workspaceColor);
    this->HardwareInformationArea->setFixedSize(0.5*width, 0.3*height);
    this->HardwareInformationAreaLayout = new QVBoxLayout(this->HardwareInformationArea);
    this->HardwareInformationAreaLayout->addWidget(this->HardwareInformation);
    this->HardwareInformationAreaLayout->addWidget(this->selfInfoArea);
    this->HardwareInformationAreaLayout->addWidget(this->tagetTestArea);
    this->HardwareInformationAreaLayout->setSpacing(0);
    this->HardwareInformationAreaLayout->setMargin(0);

    this->communicationFrameRateWindow = new PlottingBoard();
    this->communicationFrameRateWindow->setFixedSize(0.5*width, 0.3*height);

    this->communicationInformationWindow = new QWidget();
    this->communicationInformationWindowLayout = new QHBoxLayout(communicationInformationWindow);
    this->communicationInformationWindowLayout->addWidget(this->HardwareInformationArea);
    this->communicationInformationWindowLayout->addWidget(this->communicationFrameRateWindow);
    this->communicationInformationWindowLayout->setSpacing(0);
    this->communicationInformationWindowLayout->setMargin(0);

    transmissionBuffer = new QTreeWidget();
    transmissionBuffer->setStyleSheet("background-color:transparent;border:0px solid gainsboro; gridline-color: gainsboro;color:lightgray;");
    transmissionBuffer->setFixedHeight(0.3*height);
    transmissionBuffer->setColumnCount(17);
    transmissionBuffer->setHeaderLabels(transmissionHeader);//configurer les titres de chaque colone
    transmissionBuffer->header()->setStyleSheet("QHeaderView::section {background-color:transparent;color:lightgray; padding-left:0px; border: 1px solid gainsboro;}");
    transmissionBuffer->headerItem()->setTextAlignment(0, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(1, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(2, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(3, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(4, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(5, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(6, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(7, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(8, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(9, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(10, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(11, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(12, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(13, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(14, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(15, Qt::AlignCenter);
    transmissionBuffer->headerItem()->setTextAlignment(16, Qt::AlignCenter);

    transmissionBuffer->setColumnWidth(0, 60);
    transmissionBuffer->setColumnWidth(1, 50);
    transmissionBuffer->setColumnWidth(2, 50);
    transmissionBuffer->setColumnWidth(3, 50);
    transmissionBuffer->setColumnWidth(4, 70);
    transmissionBuffer->setColumnWidth(5, 50);
    transmissionBuffer->setColumnWidth(6, 50);
    transmissionBuffer->setColumnWidth(7, 50);
    transmissionBuffer->setColumnWidth(8, 50);
    transmissionBuffer->setColumnWidth(9, 50);
    transmissionBuffer->setColumnWidth(10, 50);
    transmissionBuffer->setColumnWidth(11, 50);
    transmissionBuffer->setColumnWidth(12, 50);
    transmissionBuffer->setColumnWidth(13, 50);
    transmissionBuffer->setColumnWidth(14, 50);
    transmissionBuffer->setColumnWidth(15, 50);
    transmissionBuffer->setColumnWidth(16, 20);
    transmissionBuffer->setSelectionBehavior ( QAbstractItemView::SelectRows);

    receptionBuffer = new QTreeWidget();
    receptionBuffer->setStyleSheet("background-color:transparent;border:0px solid gainsboro; gridline-color: gainsboro;color:lightgray;");
    receptionBuffer->setFixedHeight(0.3*height);
    receptionBuffer->setColumnCount(17);
    receptionBuffer->setHeaderLabels(transmissionHeader);//configurer les titres de chaque colone
    receptionBuffer->header()->setStyleSheet("QHeaderView::section {background-color:transparent;color:lightgray; padding-left:0px; border: 1px solid gainsboro;}");

    receptionBuffer->headerItem()->setTextAlignment(0, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(1, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(2, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(3, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(4, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(5, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(6, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(7, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(8, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(9, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(10, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(11, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(12, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(13, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(14, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(15, Qt::AlignCenter);
    receptionBuffer->headerItem()->setTextAlignment(16, Qt::AlignCenter);

    receptionBuffer->setColumnWidth(0, 60);
    receptionBuffer->setColumnWidth(1, 50);
    receptionBuffer->setColumnWidth(2, 50);
    receptionBuffer->setColumnWidth(3, 50);
    receptionBuffer->setColumnWidth(4, 70);
    receptionBuffer->setColumnWidth(5, 50);
    receptionBuffer->setColumnWidth(6, 50);
    receptionBuffer->setColumnWidth(7, 50);
    receptionBuffer->setColumnWidth(8, 50);
    receptionBuffer->setColumnWidth(9, 50);
    receptionBuffer->setColumnWidth(10, 50);
    receptionBuffer->setColumnWidth(11, 50);
    receptionBuffer->setColumnWidth(12, 50);
    receptionBuffer->setColumnWidth(13, 50);
    receptionBuffer->setColumnWidth(14, 50);
    receptionBuffer->setColumnWidth(15, 50);
    receptionBuffer->setColumnWidth(16, 20);
    receptionBuffer->setSelectionBehavior ( QAbstractItemView::SelectRows);

    transmissionArea = new QGroupBox();
    transmissionArea->setFixedHeight(0.35*height);
    transmissionArea->setTitle("transmission buffer");
    transmissionArea->setFont(QFont("Helvetica",12, QFont::AnyStyle, true));
    transmissionArea->setStyleSheet(groupBoxStyleSheet);
    transmissionAreaLayout = new QHBoxLayout(transmissionArea);
    transmissionAreaLayout->addWidget(transmissionBuffer);
    transmissionAreaLayout->setSpacing(0);
    transmissionAreaLayout->setMargin(0);

    receptionArea = new QGroupBox();
    receptionArea->setFixedHeight(0.35*height);
    receptionArea->setTitle("reception buffer");
    receptionArea->setFont(QFont("Helvetica",12, QFont::AnyStyle, true));
    receptionArea->setStyleSheet(groupBoxStyleSheet);
    receptionAreaLayout = new QHBoxLayout(receptionArea);
    receptionAreaLayout->addWidget(receptionBuffer);
    receptionAreaLayout->setSpacing(0);
    receptionAreaLayout->setMargin(0);

    this->CommunicationStatesContainerLayout = new QVBoxLayout(this);
    this->CommunicationStatesContainerLayout->addWidget(communicationInformationWindow);
    this->CommunicationStatesContainerLayout->addWidget(transmissionArea);
    this->CommunicationStatesContainerLayout->addWidget(receptionArea);
    this->CommunicationStatesContainerLayout->setSpacing(0);
    this->CommunicationStatesContainerLayout->setMargin(0);
}

//! ---------------------------------------------------------------------------------------------------
//!
//! \brief CommunicationStatesContainer::embellshingIHM
//!
//!
void CommunicationStatesContainer::embellshingIHM(){
    this->HardwareInformation->setStyleSheet("QTableWidget {selection-background-color:lightgray;color:beige; border:0px solid lightgray; gridline-color: lightgray; background-color:"+this->workspaceColor+"}"
                                             "QTableWidgetItem::item{background-color:transparent; color:beige;}"
                                             "QHeaderView::section {background-color: whitesmoke; color:"+workspaceColor+";border:0px solid lightgray}");

    this->communicationFrameRateWindow->setWorkSpaceColor(this->workspaceColor);
    this->communicationFrameRateWindow->setBasePenProperty(QColor("gainsboro"), 1);
    this->communicationFrameRateWindow->setTickPenProperty(QColor("lightgray"), 0.5);
    this->communicationFrameRateWindow->setGridStyle(false,"gray","lightgray",true, workspaceColor, "lightgray");
}

//! ---------------------------------------------------------------------------------------------------
//!
//! \brief CommunicationStatesContainer::setConnections
//!
void CommunicationStatesContainer::setConnections(){
    this->connect(this->testConnectBackButton, SIGNAL(clicked()), this, SLOT(connectedToServer()));
}

//! ----------------------------------------------------------------------------------------------------
//!
//! \brief CommunicationStatesContainer::connectedToServer
//!
void CommunicationStatesContainer::connectedToServer(){
    emit connectedToServerSignal(this->targetIpAddressLineEdit->text(), this->targetListenPortLineEdit->text().toInt());
}
