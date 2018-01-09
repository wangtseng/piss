#include "pissMainWindow.h"

//!---------------------------------------------------------------------------------------
//!
//! le  tâche qui va être remplacer par le version cpp
//!
//! lancer le process aui fait l'acquisition des images ct du loin
//!
//! \brief pissMainWindow::CommunicationTask::run
//!
void pissMainWindow::CommunicationTask::run(){
    system("python C:\\projet\\qcxx\\piss_server\\main.py");
}

/**
 * @brief pissMainWindow::pissMainWindow
 * @param dispatcher
 */
pissMainWindow::pissMainWindow(SystemDispatcher* dispatcher): QWidget(){

    this->systemDispatcher = dispatcher;
    this->initVariable();
    this->constructIHM();
    this->globalBackgroundColorSetting();
    this->setConnections();
    this->drawBackground();
    this->onSystemWidgetOptionReleased();

    //task.start();
}

//!---------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::initVariable
//!
void pissMainWindow::initVariable(){

    //! -------------------------------------------------------
    //! Faire la decoration de l'IHM
    //! -------------------------------------------------------
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setWindowOpacity(1);
    this->setMouseTracking(true);
    this->setAutoFillBackground(true);

    //! -------------------------------------------------------
    //! Des variable utile pour manipuler l'IHM
    //! -------------------------------------------------------
    this->englishCaracterStyle = new QFont("Segoe UI", 8, QFont::AnyStyle, true);
    this->globalBackgroundColor = "rgb(45,45,45)";
    this->globalWorkSpaceColor = "rgb(45,45,45)";
    this->globalFontColor = "orange";//darkgray

    //! -------------------------------------------------------
    //! Des procedure afin de recuperer les tailles des ecrans;
    //! -------------------------------------------------------
    this->desktop = QApplication::desktop();
    this->screen_count = desktop->screenCount();
    this->primary_screen = desktop->primaryScreen();

    this->screen = new Screen[screen_count];
    for(unsigned char i = 0; i < screen_count; i++){
        this->screen[i].screenIndex = primary_screen + i;
        this->screen[i].rect = desktop->screenGeometry(primary_screen + i);
    }

    this->primary_screen_width = screen[0].rect.width();
    this->primary_screen_height = screen[0].rect.height();

    this->the_tab_style =   "QTabBar::tab{background: " + globalBackgroundColor + "; color: " + globalFontColor + "; padding: 0px; border-top: 0px solid gainsboro; border-bottom: 0px solid orange; height: "+QString::number(primary_screen_height*0.03)+"px; width: "+QString::number(primary_screen_width*0.15)+"px;  } "
                            "QTabBar::tab:selected {background: " + globalWorkSpaceColor + "; color: " + globalFontColor + "; padding: 0px; border-top: 0px solid gainsboro; border-bottom: 1px solid orange;} "
                            "QTabWidget::pane { border: 0; }";

    //!----------------------------------------------------------------------------------------------------
    //! status bar area
    //!----------------------------------------------------------------------------------------------------
    this->algorithmTestPlatform = new AlgorithmTestPlatform(this->systemDispatcher,
                                                            this->primary_screen_width,
                                                            this->primary_screen_height*0.1,
                                                            this->englishCaracterStyle);

    this->patientInformationWidget = new PatientInformationWidget(this->primary_screen_width,
                                                                  this->primary_screen_height);

    this->surgeryTimer = new QTime();
    //!
    //!----------------------------------------------------------------------------------------------------
    //! ihm pour configurer les parametre des systems
    //!----------------------------------------------------------------------------------------------------
    systemOptionWindow = new SystemOptions();
}

//!---------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::getAlgorithmTestPlatform
//! \return
//!
AlgorithmTestPlatform* pissMainWindow::getAlgorithmTestPlatform(){
    return this->algorithmTestPlatform;
}

//! ---------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::globalBackgroundColorSetting
//!
void pissMainWindow::globalBackgroundColorSetting(){
    controlBoard->setStyleSheet("background-color:"+this->globalBackgroundColor);
    this->algorithmTestPlatform->setBackgroundColor(this->globalBackgroundColor);
    systemInformationBoardWidget->setStyleSheet("background-color:"+this->globalBackgroundColor);
    drawBackground();
}

//!---------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::constructIHM
//!
void pissMainWindow::constructIHM(){

    //!----------------------------------------------------------------------------------------------------------------------------
    //! le page pour visualiser les informations des maladies, aussi pour choisir la bonne personne qui doit commencer le th??rapy
    //!----------------------------------------------------------------------------------------------------------------------------
    patientsWidget = new PatientsWidget(  this->systemDispatcher,
                                          this->algorithmTestPlatform,
                                          this->patientInformationWidget,
                                          this->surgeryTimer,
                                          this->englishCaracterStyle,
                                          this->primary_screen_width*0.985,
                                          this->primary_screen_height*0.87,
                                          this->globalWorkSpaceColor,
                                          this->globalFontColor);

    //!----------------------------------------------------------------------------------------------------------------------------
    //! surveillance sur les etats du system
    //!----------------------------------------------------------------------------------------------------------------------------
    surgerySystemWidget = new SurgerySystemWidget(this->primary_screen_width*0.985,
                                                  this->primary_screen_height*0.87,
                                                  this->systemDispatcher,
                                                  this->algorithmTestPlatform,
                                                  this->globalWorkSpaceColor);

    //!----------------------------------------------------------------------------------------------------------------------------
    //! historique
    //!----------------------------------------------------------------------------------------------------------------------------
    replaysWidget = new ReplaysWidget();

    widgetsContainer = new QTabWidget();
    widgetsContainer->setFont(QFont("Segoe UI", 13, QFont::DemiBold, true));

    widgetsContainer->setStyleSheet(the_tab_style);
    widgetsContainer->setFixedHeight(primary_screen_height*0.9);
    widgetsContainer->insertTab(0,patientsWidget, "patient");
    widgetsContainer->insertTab(1,surgerySystemWidget, "system");
    widgetsContainer->insertTab(2,replaysWidget, "documentation");

    systemInformationBoardWidget = new QWidget();
    systemInformationBoardWidgetLayout = new QVBoxLayout(systemInformationBoardWidget);
    systemInformationBoardWidgetLayout->addWidget(widgetsContainer);
    systemInformationBoardWidgetLayout->setSpacing(0);
    systemInformationBoardWidgetLayout->setMargin(0);

    //!------------------------------------------------------------------------------------------
    //! controlBoard:
    //!------------------------------------------------------------------------------------------
    closeButton = new QPushButton();
    closeButton->setIcon(QIcon(":/images/close.png"));
    closeButton->setIconSize(QSize(this->primary_screen_width*0.015,this->primary_screen_width*0.015));
    closeButton->setStyleSheet("background-color:transparent");
    closeButton->setFixedSize(this->primary_screen_width*0.015, this->primary_screen_width*0.015);

    systemConfigurationButton = new QPushButton();
    systemConfigurationButton->setIcon(QIcon(":/images/system_configuration.png"));
    systemConfigurationButton->setIconSize(QSize(this->primary_screen_width*0.015,this->primary_screen_width*0.015));
    systemConfigurationButton->setStyleSheet("background-color:transparent");
    systemConfigurationButton->setFixedSize(this->primary_screen_width*0.015, this->primary_screen_width*0.015);

    controlAreaItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);

    controlBoard = new QWidget();
    controlBoard->setFixedWidth(this->primary_screen_width*0.015);
    controlBoardLayout = new QVBoxLayout(controlBoard);
    controlBoardLayout->addWidget(closeButton);
    controlBoardLayout->addWidget(systemConfigurationButton);
    controlBoardLayout->addItem(controlAreaItem);
    controlBoardLayout->setSpacing(0);
    controlBoardLayout->setMargin(0);

    //!----------------------------------------------------------------------------------------------------
    //! main platform
    //!----------------------------------------------------------------------------------------------------
    mainPlatform = new QWidget();
    mainPlatformLayout = new QHBoxLayout(mainPlatform);
    mainPlatformLayout->addWidget(systemInformationBoardWidget);
    mainPlatformLayout->addWidget(controlBoard);
    mainPlatformLayout->setSpacing(0);
    mainPlatformLayout->setMargin(0);

    //!----------------------------------------------------------------------------------------------------
    //! main window
    //!----------------------------------------------------------------------------------------------------
    pissMainWindowLayout =  new QVBoxLayout(this);
    pissMainWindowLayout->addWidget(mainPlatform);
    pissMainWindowLayout->addWidget(algorithmTestPlatform);
    pissMainWindowLayout->setSpacing(1);
    pissMainWindowLayout->setMargin(0);
}

//! ---------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::findPatientExisted
//!
void pissMainWindow::findPatientExisted(){
   patientsWidget->findPatientExisted();
}
//! -----------------------------------------------------------------------------------------
//!
//! \brief AnalyserMainWindow::drawBackground
//!
void pissMainWindow::drawBackground(){
    this->setStyleSheet("background:"+this->globalWorkSpaceColor + "; color:"+this->globalFontColor);
}

//!------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::setConnectios
//!
void pissMainWindow::setConnections(){

    this->connect(this->patientsWidget, SIGNAL(surgeryLaunchButtonCicked()), this, SLOT(close()));
    this->connect(this->patientsWidget, SIGNAL(surgeryFinished()), this, SLOT(showFullScreen()));
    this->connect(closeButton, SIGNAL(clicked()), this, SLOT(closeSystem()));
    this->connect(systemConfigurationButton, SIGNAL(clicked()), this, SLOT(configurerLeSysteme()));
    this->connect(this->systemOptionWindow, SIGNAL(confirm()), this, SLOT(updateIHM()));

}

//!------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::updateIHM
//!
void pissMainWindow::updateIHM(){

    this->globalBackgroundColor = this->systemOptionWindow->getSystemGlobalBackgroundColor();
    this->globalWorkSpaceColor = this->systemOptionWindow->getSystemGlobalWorkSpaceColor();

    globalBackgroundColorSetting();
    widgetsContainer->setStyleSheet(the_tab_style);
    if(mainOptionStates.historyWidgetOptionState){
        this->onHistoryWidgetOptionReleased();
    }
    else if(mainOptionStates.patientsWidgetOptionState){
        this->onPatientsWidgetOptionReleased();
    }
    else if(mainOptionStates.systemWidgetOptionState){
        this->onSystemWidgetOptionReleased();
    }

    patientsWidget->setWorkSpaceColor(this->globalWorkSpaceColor);

}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::configurerLeSysteme
//!
void pissMainWindow::configurerLeSysteme(){
    systemOptionWindow->show();
}

//!----------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::initVisualizationComponents
//!
void pissMainWindow::initVisualizationComponents(){
    volumeMapper = vtkFixedPointVolumeRayCastMapper::New();
    volume = vtkVolume::New();
    renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
}

//!----------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::display
//!
void pissMainWindow::display(){
    this->showFullScreen();
}

//!----------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::setSystemDispatecher
//! \param dispatcher
//!
void pissMainWindow::setSystemDispatcher(SystemDispatcher* dispatcher){
    this->systemDispatcher = dispatcher;
}

//!----------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::displayMraImage
//! \param input
//!
void pissMainWindow::displayMraImage(vtkImageData* input){
    //this->volumeMapper->Set
    //this->volumeMapper->SetVolumeRayCastFunction(compositeFunction);
    this->volumeMapper->SetInputData(input);
    this->volume->SetMapper(volumeMapper);
    this->renderer->AddVolume(volume);
    this->renderer->SetBackground(0.1, 0.21, 0.31);
    this->renderWindow->AddRenderer(renderer);
    this->renderer->ResetCamera();
    this->mraImageDisplayWindow->SetRenderWindow(this->renderWindow);
}

//!----------------------------------------------------------------------------------------
//!
//! \brief Widget::readImageFileFrom
//! \param path
//!
void pissMainWindow::readImageFileFrom(QString path){
    mhdImagePath = path;
    mraImage = new IgssImage();
    mhdFileReader = new MRAFileReader();
    mhdFileReader->doParseMHDFileToIgssImage(mhdImagePath ,mraImage);

}

//!----------------------------------------------------------------------------------------
//!
//! \brief Widget::readImageFromVtkConvert
//!
void pissMainWindow::readImageFromVtkConvert()
{
    igssImage = new IgssImage();
    vtkImage = vtkImageData::New();
    vtkImage->SetDimensions(25,25,25);
    vtkImage->SetSpacing(1.0,1.0,1.0);
    vtkImage->SetOrigin(0.0,0.0,0.0);
    vtkImage->AllocateScalars(VTK_UNSIGNED_SHORT,1);
    unsigned short *ptr = (unsigned short*)vtkImage->GetScalarPointer();
    for(long i=0;i<25*25*25;i++)
    {
        *ptr++ = i;
    }
    igssVtkImageConverter = new IgssVtkImageConverter();
    igssVtkImageConverter->VtkToIgss(vtkImage,igssImage);
}

//!----------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::closeSystem
//!
void pissMainWindow::closeSystem(){
    if (!(QMessageBox::information(this,tr("Igss Control View"),tr("---------------------------------------------------------------\n Do you really want to log out Image-guided surgery system?\n---------------------------------------------------------------"),tr("Yes"),tr("No")))){
        this->close();
        this->systemOptionWindow->close();
        //delete &task;
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onPatientsWidgetOptionHovered
//!
void pissMainWindow::onPatientsWidgetOptionHovered(){
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onPatientsWidgetOptionClicked
//!
void pissMainWindow::onPatientsWidgetOptionClicked(){
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onPatientsWidgetOptionReleased
//!
void pissMainWindow::onPatientsWidgetOptionReleased(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onPatientsWidgetOptionLeaved
//!
void pissMainWindow::onPatientsWidgetOptionLeaved(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onSystemWidgetOptionHovered
//!
void pissMainWindow::onSystemWidgetOptionHovered(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onSystemWidgetOptionClicked
//!
void pissMainWindow::onSystemWidgetOptionClicked(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onSystemWidgetOptionReleased
//!
void pissMainWindow::onSystemWidgetOptionReleased(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onSystemWidgetOptionLeaved
//!
void pissMainWindow::onSystemWidgetOptionLeaved(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onHistoryWidgetOptionHovered
//!
void pissMainWindow::onHistoryWidgetOptionHovered(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onHistoryWidgetOptionClicked
//!
//!
void pissMainWindow::onHistoryWidgetOptionClicked(){
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onHistoryWidgetOptionReleased
//!
void pissMainWindow::onHistoryWidgetOptionReleased(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::onHistoryWidgetOptionLeaved
//!
void pissMainWindow::onHistoryWidgetOptionLeaved(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::mouseMoveEvent
//! \param event
//!
void pissMainWindow::mouseMoveEvent(QMouseEvent *event){
//    if(buttonGaucheDuSourisClique == true){
//        mouseMovingPosition = event->globalPos();
//        this->move(this->pos() + mouseMovingPosition - mousePosition);
//        mousePosition = mouseMovingPosition;
//    }
    event->ignore();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::mousePressEvent
//! \param event
//!
void pissMainWindow::mousePressEvent(QMouseEvent *event){
//    if(event->button() == Qt::LeftButton){
//       if (!((event->y() < 5) || (event->x() <5))){
//           mousePosition = event->globalPos();
//           buttonGaucheDuSourisClique = true;
//       }
//    }
    event->ignore();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief IgssMainWindow::mouseReleaseEvent
//! \param event
//!
void pissMainWindow::mouseReleaseEvent(QMouseEvent *event){
//    if(event->button() == Qt::LeftButton){
//        buttonGaucheDuSourisClique = false;
//    }
    event->ignore();
}

