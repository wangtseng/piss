#include "PatientsWidget.h"


/**
 * @brief PatientsWidget::PatientsWidget
 * @param screen_count
 * @param dispatcher
 * @param algorithmTestPlatform
 * @param patientInformationWidget
 * @param surgeryTimer
 * @param caracterStyle
 * @param appWidth
 * @param appHeight
 * @param workspaceColor
 */
PatientsWidget::PatientsWidget(SystemDispatcher* dispatcher,
                               AlgorithmTestPlatform *algorithmTestPlatform,
                               PatientInformationWidget* patientInformationWidget,
                               QTime* surgeryTimer,
                               QFont *caracterStyle,
                               int appWidth,
                               int appHeight,
                               QString workspaceColor,
                               QString globalFontColor) : QWidget(){

    this->dispatcher = dispatcher;
    this->algorithmTestPlatform = algorithmTestPlatform;
    this->patientInformationWidget = patientInformationWidget;
    this->surgeryTimer = surgeryTimer;
    this->caracterStyle = caracterStyle;
    this->appWidth = appWidth;
    this->appHeight = appHeight;
    this->workspaceColor = workspaceColor;
    this->globalFontColor = globalFontColor;

    this->initVariable();
    this->constructIHM();
    this->setConnections();
    this->setWorkSpaceColor(this->workspaceColor);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::setConnections
//!
void PatientsWidget::setConnections(){
    this->connect(this->surgeryLaunchButton,       SIGNAL(clicked()),               this,  SLOT(launchSurgery()));
    this->connect(this->simpleModeButton,          SIGNAL(clicked()),               this,  SLOT(simpleViewButtonClicked()));
    this->connect(this->cdRomParseButton,          SIGNAL(clicked()),               this,  SLOT(doParseCdRom()));
    this->connect(this->addPatientButton,          SIGNAL(clicked()),               this,  SLOT(addPatient()));
    this->connect(this->leftSelectButton,          SIGNAL(clicked()),               this,  SLOT(doLeftSelect()));
    this->connect(this->rightSelectButton,         SIGNAL(clicked()),               this,  SLOT(doRightSelect()));
    this->connect(this->guidewareMovementButton,   SIGNAL(clicked()),               this,  SLOT(onGuidewareMovementButtonClicked()));
    this->connect(this->cameraFlyThroughButton,    SIGNAL(clicked()),               this,  SLOT(onCameraFlyThroughButtonClicked()));
    this->connect(this->flyThroughTimer,           SIGNAL(timeout()),               this,  SLOT(flyThrough()));
    this->connect(this->cutButton,                 SIGNAL(clicked()),               this,  SLOT(onCutButtonClicked()));
    this->connect(this->clearCenterLineButton,     SIGNAL(clicked()),               this,  SLOT(onClearCenterLineButtonClicked()));
    this->connect(this->cuttingLayerOption,        SIGNAL(valueChanged(int)),       this,  SLOT(cuttingLayerOptionChanged(int)));
    this->connect(this->autoVolumeRenderingButton, SIGNAL(clicked()),               this,  SLOT(autoVolumeRenderingButtonClicked()));
    this->connect(this->threeSliceEnableButton,    SIGNAL(clicked()),               this,  SLOT(threeSliceEnableButtonClicked()));
    this->connect(this->tissueExtractionButton,    SIGNAL(clicked()),               this,  SLOT(tissueExtractionButtonClicked()));
    this->connect(this->modalityOptionComboBox,    SIGNAL(currentIndexChanged(int)),this,  SLOT(modalityOptionComboBoxChanged(int)));
    this->connect(this->reconstructButton,         SIGNAL(clicked()),               this,  SLOT(reconstructButtonClicked()));
    this->connect(this->vesselModelingLineEdit,    SIGNAL(returnPressed()),         this,  SLOT(vesselModelingLineEditPressed()));

    //! update coords as we move through the window
    this->currentPatientVolumeDataAnalyseAreaConnections->Connect(currentPatientVolumeDataAnalyseArea->GetRenderWindow()->GetInteractor(), vtkCommand::MouseMoveEvent, this, SLOT(updateCoords(vtkObject*)));
    //this->currentPatientVolumeDataAnalyseAreaConnections->Connect(currentPatientVolumeDataAnalyseArea->GetRenderWindow()->GetInteractor(), vtkCommand::LeftButtonPressEvent, this, SLOT(onImageMouseLeftButtonPressed(vtkObject*)));
    //this->currentPatientVolumeDataAnalyseAreaConnections->Connect(currentPatientVolumeDataAnalyseArea->GetRenderWindow()->GetInteractor(), vtkCommand::LeftButtonReleaseEvent, this, SLOT(onImageMouseLeftButtonReleased(vtkObject*)));
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::updateCoords
//! \param obj
//!
void PatientsWidget::updateCoords(vtkObject* obj){
  // get interactor
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);

  // get event position
  int event_pos[2];
  iren->GetEventPosition(event_pos);
  // update label
  QString str;
  str.sprintf("x=%d : y=%d", event_pos[0], event_pos[1]);
  //this->algorithmTestPlatform->setSystemStatus(str);
  //qDebug()<<str;
  //coord->setText(str);

  this->flyThroughRenderer->SetActiveCamera(originVolumeDataCamera);

  this->originVolumeDataRenderer->ResetCamera();
  this->flyThroughRenderer->ResetCamera();
  this->interventionalRoutePlanningArea->update();
  this->currentPatientVolumeDataAnalyseArea->update();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::launchSurgery
//!
void PatientsWidget::launchSurgery(){

    //! to tell the piss main window to close itself
    emit surgeryLaunchButtonCicked();

    //! fetch the pointer of the patient selected, and then pass it into each sub-ihm of the system
    Patient *patientHandling = this->dispatcher->getPatientById(waitingPatientsIDQueue[numberOfPatient-1].toInt(0,10));

    this->dispatcher->setPatientHandling(waitingPatientsIDQueue[numberOfPatient-1].toInt(0,10));

    //! exchange file to connect python communication module and piss
    this->configuratonFilePath = "C:\\Users\\cheng\\Documents\\CanalyserWorkspace\\CanalyserMetadata\\configuration.txt";

    //! Des procedure afin de recuperer les tailles des ecrans;
    this->desktop = QApplication::desktop();
    this->screen_count = desktop->screenCount();
    this->primary_screen = desktop->primaryScreen();

    this->screen = new SScreen[screen_count];
    for(unsigned char i = 0; i < screen_count; i++){
        this->screen[i].screenIndex = primary_screen + i;
        this->screen[i].rect = desktop->screenGeometry(primary_screen + i);
    }

    this->primary_screen_width = screen[0].rect.width();
    this->primary_screen_height = screen[0].rect.height();

    //this->algorithmTestPlatform->setSystemStatus("screen_count:"+QString::number(screen_count));
    //! three humain-machine interface for intra-operation
    if(screen_count == 1){
        controlConsoleWindow = new ControlConsoleWindow(this->screen[0].rect, this->surgeryTimer, this->dispatcher, this->algorithmTestPlatform, this->workspaceColor, this->configuratonFilePath);
        controlConsoleWindow->setPatientHandling(patientHandling);
        controlConsoleWindow->update();
        connect(controlConsoleWindow, SIGNAL(missionAccomplishiment(QString)), this, SLOT(terminateSurgery(QString)));
        controlConsoleWindow->showFullScreen();
    }
    else if(screen_count == 2){
        surgeryPlanWindow = new SurgeryPlanWindow(this->screen[1].rect, this->surgeryTimer, this->dispatcher, this->workspaceColor, this->configuratonFilePath);
        surgeryPlanWindow->setPatientHandling(patientHandling);
        surgeryPlanWindow->update();

        controlConsoleWindow = new ControlConsoleWindow(this->screen[0].rect, this->surgeryTimer, this->dispatcher, this->algorithmTestPlatform, this->workspaceColor, this->configuratonFilePath);
        controlConsoleWindow->setPatientHandling(patientHandling);
        controlConsoleWindow->update();

        connect(controlConsoleWindow, SIGNAL(missionAccomplishiment(QString)), this, SLOT(terminateSurgery(QString)));
        surgeryPlanWindow->displayWindow();
        controlConsoleWindow->showFullScreen();
    }
    else if(screen_count == 3){
        surgeryPlanWindow = new SurgeryPlanWindow(this->screen[1].rect, this->surgeryTimer, this->dispatcher, this->workspaceColor, this->configuratonFilePath);
        surgeryPlanWindow->setPatientHandling(patientHandling);
        surgeryPlanWindow->update();

        guidewareTrackingWindow = new GuidewareTrackingWindow(this->screen[2].rect, this->dispatcher, this->workspaceColor, this->configuratonFilePath);
        guidewareTrackingWindow->setPatientHandling(patientHandling);
        guidewareTrackingWindow->update();

        controlConsoleWindow = new ControlConsoleWindow(this->screen[0].rect, this->surgeryTimer, this->dispatcher, this->algorithmTestPlatform, this->workspaceColor, this->configuratonFilePath);
        controlConsoleWindow->setPatientHandling(patientHandling);
        controlConsoleWindow->update();
        connect(controlConsoleWindow, SIGNAL(missionAccomplishiment(QString)), this, SLOT(terminateSurgery(QString)));
        surgeryPlanWindow->displayWindow();
        guidewareTrackingWindow->displayWindow();
        controlConsoleWindow->showFullScreen();
    }

    //! launch surgery timer
    //this->surgeryTimer->start();
    //surgeryPlanWindow->setStartTime(this->surgeryTimer->elapsed());
    //qDebug()<<this->surgeryTimer->elapsed();
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::resetConfigurationFile
//!
void PatientsWidget::resetConfigurationFile(){
    QFile f(this->configuratonFilePath);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cout << "Open failed." << endl;
    }

    QTextStream txtOutput(&f);

    txtOutput << "state:off" << endl;
    txtOutput << "name:unknown" << endl;
    txtOutput << "type:unknown" << endl;
    txtOutput << "path:unknown" << endl;

    f.close();
}

void PatientsWidget::documenter(QString path){
    QDateTime time = QDateTime::currentDateTime();
    QString current_date = time.toString("yyyy-MM-dd-hh-mm-ss");
    this->copyPathAndDeleteFiles(path, path+"history/"+current_date+"/");
}

void PatientsWidget::copyPathAndDeleteFiles(QString sourceDir, QString destinationDir)
{
    qDebug()<<"surgeryTerminated:"<<destinationDir;
    QString operation_folder1 = "reconstruct";
    QString operation_folder2 = "navi";

    QString sourceDir1 = sourceDir + "/" + operation_folder1;
    QString sourceDir2 = sourceDir + "/" + operation_folder2;

    QString destinationDir1 = destinationDir + "/" + operation_folder1;
    QString destinationDir2 = destinationDir + "/" + operation_folder2;

    QDir originDirectory1(sourceDir1);
    QDir originDirectory2(sourceDir2);

    if (! originDirectory1.exists() || ! originDirectory2.exists())
    {
        std::cout << "origin files don't exist!";
    }

    originDirectory1.mkpath(destinationDir1);
    originDirectory2.mkpath(destinationDir2);

    foreach (QString fileName, originDirectory1.entryList(QDir::Files))
    {
        QFile::copy(sourceDir1 + "/" + fileName, destinationDir1 + "/" + fileName);
    }

    foreach (QString fileName, originDirectory2.entryList(QDir::Files))
    {
        QFile::copy(sourceDir2 + "/" + fileName, destinationDir2 + "/" + fileName);
    }

    //remove files
    originDirectory1.setNameFilters(QStringList() << "*.raw*");
    originDirectory1.setFilter(QDir::Files);

    foreach(QString dirFile, originDirectory1.entryList())
    {
        originDirectory1.remove(dirFile);
    }

    originDirectory2.setNameFilters(QStringList() << "*.raw*");
    originDirectory2.setFilter(QDir::Files);
    QString filter_string = "NAV";

    foreach(QString dirFile, originDirectory2.entryList())
    {
        if(dirFile.startsWith(filter_string)) originDirectory2.remove(dirFile);
    }

}

//!-------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief pissMainWindow::surgeryTerminated
//!
void PatientsWidget::terminateSurgery(QString path){

    if(screen_count == 1){
        this->resetConfigurationFile();
        this->controlConsoleWindow->close();
    }
    else if(screen_count == 2){
        this->resetConfigurationFile();
        this->surgeryPlanWindow->close();
        this->surgeryPlanWindow->documenter();
        this->controlConsoleWindow->close();
    }
    else if(screen_count == 3){
        this->documenter((path));
        this->resetConfigurationFile();
        this->surgeryPlanWindow->close();
        this->guidewareTrackingWindow->close();
        this->controlConsoleWindow->close();
    }
    this->algorithmTestPlatform->setSystemStatus("the previous therapy terminated");
    emit surgeryFinished();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::initVariable
//!
void PatientsWidget::initVariable(){
    this->numberOfPatient   = 0;
    this->dicomCDRomReader  = new DicomCDRomReader();
    this->dicomCDRomReader->setAlgorithmTestPlatform(this->algorithmTestPlatform);
    this->photoLabelStyleSheet = "border: 0px solid darkgray;border-radius: 0px;padding: 2 2px;background-color: transparent; color:" + globalFontColor;
    this->labelStyleSheet = "border: 0px solid darkgray;border-radius: 0px;background-color: transparent; color: "+globalFontColor;
    this->textEditStyleSheet = "border: 0px solid darkgray; color: "+ globalFontColor +"; background-color:"+this->workspaceColor;

    this->superviseWindow = new SuperviseWindow(appWidth, appHeight);

    this->volumeMapper = vtkFixedPointVolumeRayCastMapper::New();
    this->volume = vtkVolume::New();
    this->renderWindow = vtkSmartPointer<vtkRenderWindow>::New() ;
    this->originVolumeDataRenderer = vtkSmartPointer<vtkRenderer>::New();

    this->renderWindow->AddRenderer(originVolumeDataRenderer);
    this->originVolumeDataRenderer->SetBackground((1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255);

    this->volumeProperty = vtkVolumeProperty::New();
    this->volumeProperty->ShadeOff();
    this->volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFixedSize(this->appWidth, this->appHeight);

    vessel = vtkPoints::New();

    flyThroughTimer = new QTimer();
    flyThroughCpt = 0;

    flyThroughRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    flyThroughRenderer = vtkSmartPointer<vtkRenderer>::New();
    flyThroughRenderWindow->AddRenderer(flyThroughRenderer);

    shiftScaleVolumeData = vtkImageShiftScale::New();

    cuttingLayerOptionActor = vtkActor::New();
    originVolumeDataCamera = this->originVolumeDataRenderer->GetActiveCamera();
    flyThroughCamera = this->flyThroughRenderer->GetActiveCamera();


    //this->flyThroughRenderer->SetActiveCamera(originVolumeDataCamera);
    //this->originVolumeDataRenderer->SetActiveCamera(flyThroughCamera);
    vesselPointCount = 0;

    centerlineActorSet.clear();
    isGuidewareMovementButtonClicked = false;

    for (int i = 0; i < 3; i++){
      riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
    }

    picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.005);

    ipwProp = vtkSmartPointer<vtkProperty>::New();

    simpleViewButtonIsClicked = false;
    tissueExtractionConfigurationBoard = new PatientWidgetConfigurationBoard();

    modalities<<"T1"<<"T2"<<"TOF";
    currentModality = "T1";

    this->currentPatientVolumeDataAnalyseAreaConnections = vtkEventQtSlotConnect::New();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::generatePointAt
//! \param x
//! \param y
//! \param z
//! \param radius
//!
void PatientsWidget::generatePointAt(double x, double y, double z, double radius){
    vtkSphereSource *pos = vtkSphereSource::New();
    pos->SetRadius(radius);
    pos->SetThetaResolution(12);
    pos->SetPhiResolution(6);
    pos->SetCenter(x,y,z);
    vtkPolyDataMapper *posMapper = vtkPolyDataMapper::New();
    posMapper->SetInputConnection(pos->GetOutputPort());
    vtkActor *posActor = vtkActor::New();
    posActor->SetMapper(posMapper);
    posActor->GetProperty()->SetColor(0,255,0);
    this->originVolumeDataRenderer->AddActor(posActor);
    this->originVolumeDataRenderer->ResetCamera();
    this->currentPatientVolumeDataAnalyseArea->update();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::commandAnalyse
//! \param cmd
//!
void PatientsWidget::commandAnalyse(QString cmd){
    if(cmd.contains("add")&&cmd.contains("at")){
        QStringList temp = cmd.split("at");
        QString temp1 = temp[0];
        QString temp2 = temp[1];
        QString point_name = temp1.split("point")[1].simplified();
        QString point_pos = temp2.split("(")[1].split(")")[0].simplified ();

        QStringList params = point_pos.split(",");
        double x = params[0].toDouble();
        double y = params[1].toDouble();
        double z = params[2].toDouble();
        double radius = params[3].toDouble();
        generatePointAt(x, y, z, radius);

        this->algorithmTestPlatform->setSystemStatus(point_name);
        this->algorithmTestPlatform->setSystemStatus(point_pos);
    }
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::vesselModelingLineEditPressed
//!
void PatientsWidget::vesselModelingLineEditPressed(){
    QString lastCommand = this->vesselModelingLineEdit->text();

    this->commandAnalyse(lastCommand);

    algorithmTestPlatform->setSystemStatus(lastCommand);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::onCutButtonClicked
//!
void PatientsWidget::onCutButtonClicked(){
    this->algorithmTestPlatform->setSystemStatus( "cut the centerline on layer: " + this->cuttingLayerOption->text()/*.toInt()*/);
    int pos = this->cuttingLayerOption->text().toInt();

    double p0[3];
    double p1[3];
    double p2[3];

    vessel->GetPoint(pos - 1, p0);
    vessel->GetPoint(pos, p1);
    vessel->GetPoint(pos + 1, p2);

    igsssCutter cutter;
    vtkActor* actor = cutter.cut(p0, p1, p2, this->shiftScaleVolumeData);

    this->flyThroughRenderer->AddActor(actor);
    this->interventionalRoutePlanningArea->update();
    this->flyThroughRenderer->SetActiveCamera(originVolumeDataCamera);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::cuttingLayerOptionChanged
//! \param value
//!
void PatientsWidget::cuttingLayerOptionChanged(int value){
    algorithmTestPlatform->setSystemStatus(QString::number(value));
    double p0[3];
    vessel->GetPoint(value, p0);

    vtkSphereSource *pos = vtkSphereSource::New();
    pos->SetRadius(0.8);
    pos->SetThetaResolution(12);
    pos->SetPhiResolution(6);
    pos->SetCenter(p0[0],p0[1],p0[2]);

    vtkPolyDataMapper *posMapper = vtkPolyDataMapper::New();
    posMapper->SetInputConnection(pos->GetOutputPort());

    cuttingLayerOptionActor->SetMapper(posMapper);
    cuttingLayerOptionActor->GetProperty()->SetColor(255,255,0);
    this->originVolumeDataRenderer->AddActor(cuttingLayerOptionActor);
    //this->renderer->ResetCamera();
    this->flyThroughRenderer->AddActor(cuttingLayerOptionActor);
    //this->flyThroughRenderer->ResetCamera();
    currentPatientVolumeDataAnalyseArea->update();
    interventionalRoutePlanningArea->update();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::flyThrough
//!
void PatientsWidget::flyThrough(){

     if(!isGuidewareMovementButtonClicked){
         double p0[3];
         double foc[3];
         vessel->GetPoint(flyThroughCpt, p0);
         vessel->GetPoint(flyThroughCpt+10, foc);

         if(!flyThroughMode){
             vtkSphereSource *pos = vtkSphereSource::New();
             pos->SetRadius(3);
             pos->SetThetaResolution(12);
             pos->SetPhiResolution(6);
             pos->SetCenter(p0[0],p0[1],p0[2]);

             vtkPolyDataMapper *posMapper = vtkPolyDataMapper::New();
             posMapper->SetInputConnection(pos->GetOutputPort());

             vtkActor *posActor = vtkActor::New();
             posActor->SetMapper(posMapper);
             posActor->GetProperty()->SetColor(255,0,0);

             centerlineActorSet.append(posActor);

             this->originVolumeDataRenderer->AddActor(posActor);
             this->originVolumeDataRenderer->ResetCamera();
             this->flyThroughRenderer->AddActor(posActor);
             this->flyThroughRenderer->ResetCamera();

         }
         else{
             originVolumeDataCamera->SetClippingRange(0.1,5);
             originVolumeDataCamera->SetFocalPoint(foc[0],foc[1],foc[2]);
             originVolumeDataCamera->SetPosition(p0[0],p0[1],p0[2]);
             originVolumeDataCamera->ComputeViewPlaneNormal();
         }
         interventionalRoutePlanningArea->update();
         currentPatientVolumeDataAnalyseArea->update();

         flyThroughCpt += 10;
         if(flyThroughCpt >= vessel->GetNumberOfPoints() - 1){
             this->flyThroughTimer->stop();
             flyThroughCpt = 0;
             if(!flyThroughMode){
                 isGuidewareMovementButtonClicked = true;
             }
         }
     }
     else{
         if(!flyThroughMode){
             this->originVolumeDataRenderer->AddActor(centerlineActorSet.at(flyThroughCpt));
             this->originVolumeDataRenderer->ResetCamera();
             this->flyThroughRenderer->AddActor(centerlineActorSet.at(flyThroughCpt));
             this->flyThroughRenderer->ResetCamera();
             interventionalRoutePlanningArea->update();
             currentPatientVolumeDataAnalyseArea->update();

             flyThroughCpt++;

             if(flyThroughCpt == centerlineActorSet.size() - 1){
                 this->flyThroughTimer->stop();
                 flyThroughCpt = 0;
             }
         }
         else{
             double p0[3];
             double foc[3];
             vessel->GetPoint(flyThroughCpt, p0);
             vessel->GetPoint(flyThroughCpt+10, foc);

             originVolumeDataCamera->SetClippingRange(0.1,5);
             originVolumeDataCamera->SetFocalPoint(foc[0],foc[1],foc[2]);
             originVolumeDataCamera->SetPosition(p0[0],p0[1],p0[2]);
             originVolumeDataCamera->ComputeViewPlaneNormal();

             currentPatientVolumeDataAnalyseArea->GetRenderWindow()->Render();
             currentPatientVolumeDataAnalyseArea->update();

             flyThroughCpt += 10;
             if(flyThroughCpt >= vessel->GetNumberOfPoints() - 1){
                 this->flyThroughTimer->stop();
                 flyThroughCpt = 0;
             }
         }
     }
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::visualiserInterventionalChemin
//! \param fileName
//! \return
//!
vtkActor* PatientsWidget::visualiserInterventionalChemin(vtkPoints* vReconstructPoints){
    vtkPolyVertex *singleVesselPoly = vtkPolyVertex::New();

    vtkUnstructuredGrid *singleVesselgrid = vtkUnstructuredGrid::New();
    vtkDataSetMapper *singleVesselMapper = vtkDataSetMapper::New();
    vtkActor *singleVesselactor = vtkActor::New();

    singleVesselPoly->GetPointIds()->SetNumberOfIds(vReconstructPoints->GetNumberOfPoints());

    for(int i = 0;i<vReconstructPoints->GetNumberOfPoints();i++){
        double p0[3];
        vReconstructPoints->GetPoint(i, p0);
        singleVesselPoly->GetPointIds()->SetId(i,i);
    }

    singleVesselgrid->SetPoints(vReconstructPoints);
    singleVesselgrid->InsertNextCell(singleVesselPoly->GetCellType(),singleVesselPoly->GetPointIds());
    singleVesselMapper->SetInputData(singleVesselgrid);
    singleVesselactor->SetMapper(singleVesselMapper);
    singleVesselactor->GetProperty()->SetColor(255.0/255, 165.0/255, 0.0/255);
    singleVesselactor->GetProperty()->SetOpacity(1);
    singleVesselactor->GetProperty()->SetPointSize(0.1);

    return singleVesselactor;
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::constructBranchByCoordinatesAndRadius
//! \param fileName
//!
QVector<vtkActor*> PatientsWidget::constructEachBranchByCoordinatesAndRadius(QString fileName){

    vtkPoints* interventionalCheminPoints = vtkPoints::New();
    vtkCellArray* vReconstructLines = vtkCellArray::New();
    vtkPolyData* vReconstructPolyData = vtkPolyData::New();
    vtkDoubleArray* vReconstructRadius = vtkDoubleArray::New();
    vtkTubeFilter* vReconstructTube = vtkTubeFilter::New();
    vtkPolyDataMapper* vReconstructMapper = vtkPolyDataMapper::New();
    vtkActor* vesselTubeActor = vtkActor::New();

    QVector<CenterLinePoint*> vesselPoints = centerLineReader.doReadComplteCenterlineFile(fileName);

    int cpt;
    for(cpt = 0; cpt<vesselPoints.size(); cpt++){
        interventionalCheminPoints->InsertPoint(cpt,vesselPoints.at(cpt)->get_abscissa(),vesselPoints.at(cpt)->get_ordinate(),vesselPoints.at(cpt)->get_isometric());
    }

    vReconstructLines->InsertNextCell(vesselPoints.size());

    for(cpt = 0; cpt<vesselPoints.size(); cpt++){
        vReconstructLines->InsertCellPoint(cpt);
    }

    vReconstructPolyData->SetPoints(interventionalCheminPoints);
    vReconstructPolyData->SetLines(vReconstructLines);

    vReconstructRadius->SetName("radius");
    vReconstructRadius->SetNumberOfTuples(vesselPoints.size());
    for(cpt = 0; cpt<vesselPoints.size(); cpt++){
        if(vesselPoints.at(cpt)->get_radius()==NULL){
            vReconstructRadius->SetTuple1(cpt,1);
        }else{
            vReconstructRadius->SetTuple1(cpt,vesselPoints.at(cpt)->get_radius());
        }
    }
    vReconstructPolyData->GetPointData()->AddArray(vReconstructRadius);
    vReconstructPolyData->GetPointData()->SetActiveScalars("radius");
    vReconstructTube->SetInputData(vReconstructPolyData);
    vReconstructTube->SetNumberOfSides(100);
    vReconstructTube->SetVaryRadiusToVaryRadiusByAbsoluteScalar();

    vReconstructMapper->SetInputConnection(vReconstructTube->GetOutputPort());
    vReconstructMapper->ScalarVisibilityOn();
    vReconstructMapper->SetScalarModeToUsePointFieldData();

    vesselTubeActor->SetMapper(vReconstructMapper);
    vesselTubeActor->GetProperty()->SetOpacity(0.5);
    vesselTubeActor->GetProperty()->SetColor(1,0,0);

    QVector<vtkActor*> interventionalPair;
    interventionalPair.append(vesselTubeActor);
    interventionalPair.append(this->visualiserInterventionalChemin(interventionalCheminPoints));

    return interventionalPair;
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::reconstructButtonClicked
//!
void PatientsWidget::reconstructButtonClicked(){
    if(this->currentModality != "TOF"){
        return;
    }
    QString temp = waitingPatientsMraPathQueue.at(this->numberOfPatient-1);
    QStringList temp1 = temp.split("mra_tridimensionel__image");

    QString targetFolder = temp1[0]+"mra_tridimensionel__image/centerlines/cerebrovascular/";

    this->algorithmTestPlatform->setSystemStatus(targetFolder);

    QDir bDir(targetFolder+"B/");
    QDir fDir(targetFolder+"F/");
    QDir lDir(targetFolder+"L/");
    QDir rDir(targetFolder+"R/");

    foreach (QString fileName, bDir.entryList(QDir::Files)){
        QVector<vtkActor*> interventionalPair = constructEachBranchByCoordinatesAndRadius(targetFolder+"B/"+fileName);
        this->flyThroughRenderer->AddActor(interventionalPair.at(0));
        this->flyThroughRenderer->AddActor(interventionalPair.at(1));
        this->originVolumeDataRenderer->AddActor(interventionalPair.at(0));
        //this->originVolumeDataRenderer->AddActor(interventionalPair.at(1));
    }

    foreach (QString fileName, fDir.entryList(QDir::Files)){
        QVector<vtkActor*> interventionalPair = constructEachBranchByCoordinatesAndRadius(targetFolder+"F/"+fileName);

        this->flyThroughRenderer->AddActor(interventionalPair.at(0));
        this->flyThroughRenderer->AddActor(interventionalPair.at(1));
        this->originVolumeDataRenderer->AddActor(interventionalPair.at(0));
    }

    foreach (QString fileName, lDir.entryList(QDir::Files)){
        QVector<vtkActor*> interventionalPair = constructEachBranchByCoordinatesAndRadius(targetFolder+"L/"+fileName);

        this->flyThroughRenderer->AddActor(interventionalPair.at(0));
        this->flyThroughRenderer->AddActor(interventionalPair.at(1));
        this->originVolumeDataRenderer->AddActor(interventionalPair.at(0));
    }

    foreach (QString fileName, rDir.entryList(QDir::Files)){
        QVector<vtkActor*> interventionalPair = constructEachBranchByCoordinatesAndRadius(targetFolder+"R/"+fileName);

        this->flyThroughRenderer->AddActor(interventionalPair.at(0));
        this->flyThroughRenderer->AddActor(interventionalPair.at(1));
        this->originVolumeDataRenderer->AddActor(interventionalPair.at(0));
    }

    //! do read stl file
//    QString target_stl = temp1[0]+"mra_tridimensionel__image/vascular/cerebro_vascular.stl";
//    vtkSTLReader* stlReader = vtkSTLReader::New();;
//    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
//    vtkActor* actor = vtkActor::New();

//    stlReader->SetFileName(target_stl.toLocal8Bit().data());
//    stlReader->Update();

//    mapper->SetInputConnection(stlReader->GetOutputPort());
//    actor->SetMapper(mapper);
//    actor->GetProperty()->SetOpacity(0.65);
//    actor->GetProperty()->SetColor(135.0/255,206.0/255,235.0/255);
//    actor->GetProperty()->SetRepresentationToWireframe();

//    this->flyThroughRenderer->AddActor(actor);

    this->interventionalRoutePlanningArea->update();
    this->currentPatientVolumeDataAnalyseArea->update();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::modalityOptionComboBoxChanged
//! \param index
//!
void PatientsWidget::modalityOptionComboBoxChanged(int index){
    this->currentModality = modalities[index];

    QString temp = waitingPatientsMraPathQueue.at(this->numberOfPatient-1);
    QStringList temp1 = temp.split(".");
    if(this->currentModality == "T1"){
        loadMRAImageFile(temp1[0]+"_T1.mhd");
        this->algorithmTestPlatform->setSystemStatus(temp1[0]+"_T1.mhd");

    }
    else if(this->currentModality == "T2"){
        loadMRAImageFile(temp1[0]+"_T2.mhd");
        this->algorithmTestPlatform->setSystemStatus(temp1[0]+"_T2.mhd");

    }
    if(this->currentModality == "TOF"){
        loadMRAImageFile(temp1[0]+"_TOF.mhd");
        this->algorithmTestPlatform->setSystemStatus(temp1[0]+"_TOF.mhd");
    }
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::tissueExtractionButtonClicked
//!
void PatientsWidget::tissueExtractionButtonClicked(){
    //! choose segmentation configuration
    this->tissueExtractionConfigurationBoard->display(QCursor::pos());


    //! customerize brain tissue segmentation methode, save
    //this->imageProcessingFactory->extractBrainCortextFrom(..)

    //! generatePolyDataActorFromMetaFile();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::threeSliceEnableButtonClicked
//!
void PatientsWidget::threeSliceEnableButtonClicked(){

}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::autoVolumeRenderingButtonClicked
//!
void PatientsWidget::autoVolumeRenderingButtonClicked(){

    QStringList statisticsList = this->dispatcher->getHistogramStatisticsFrom(this->currentVolumeData);
    int maximumGrayScaleValue = statisticsList[0].toInt(0, 10);
    int minimumGrayScaleValue = statisticsList[1].toInt(0, 10);

    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint(minimumGrayScaleValue,    0.0);
    compositeOpacity->AddPoint(minimumGrayScaleValue + (maximumGrayScaleValue - minimumGrayScaleValue)*0.2,    0.0);
    compositeOpacity->AddPoint(minimumGrayScaleValue + (maximumGrayScaleValue - minimumGrayScaleValue)*0.4,    0.5);
    compositeOpacity->AddPoint(minimumGrayScaleValue + (maximumGrayScaleValue - minimumGrayScaleValue)*0.6,    0.5);
    compositeOpacity->AddPoint(minimumGrayScaleValue + (maximumGrayScaleValue - minimumGrayScaleValue)*0.8,    1.0);
    compositeOpacity->AddPoint(maximumGrayScaleValue, 1.0);
    this->volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

    vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(minimumGrayScaleValue,     0.0,1.0,1.0);
    color->AddRGBPoint(maximumGrayScaleValue,  0.0,0.0,1.0);
    this->volumeProperty->SetColor(color);
    this->volume->SetProperty(volumeProperty);
    this->renderWindow->Render();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::onClearCenterLineButtonClicked
//!
//!    QVector <vtkActor *> centerlineActorSet;
//!    vtkActor *cuttingLayerOptionActor;
//!    vtkVolume *volume;
//!
void PatientsWidget::onClearCenterLineButtonClicked(){
    originVolumeDataRenderer->RemoveAllViewProps();
    flyThroughRenderer->RemoveAllViewProps();
    originVolumeDataRenderer->AddVolume(volume);

    this->originVolumeDataRenderer->ResetCamera();
    this->flyThroughRenderer->ResetCamera();
    currentPatientVolumeDataAnalyseArea->update();
    interventionalRoutePlanningArea->update();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::onCameraFlyThroughButtonClicked
//!
void PatientsWidget::onCameraFlyThroughButtonClicked(){
    //! delete all the actor for center line representation if GuidewareMovementButton has been Clicked
    //onClearCenterLineButtonClicked();

    flyThroughMode = true;
    if(vesselPointCount == 0){
        vesselPointCount = centerLineReader.doReadCenterLineFile("C:/Users/cheng/Documents/CanalyserWorkspace/PatientsDataware/Xv_Hs__1990_09_03/mra_tridimensionel__image/centerlines/cerebrovascular/R/R000001.txt", vessel);
        this->cuttingLayerOption->setMinimum(0);
        this->cuttingLayerOption->setMaximum(vesselPointCount-1);
    }
    this->flyThroughTimer->start(500);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::onGuidewareMovementButtonClicked
//!
void PatientsWidget::onGuidewareMovementButtonClicked(){

    //onClearCenterLineButtonClicked();

    flyThroughMode = false;

    if(vesselPointCount == 0){
        vesselPointCount = centerLineReader.doReadCenterLineFile("C:/Users/cheng/Documents/CanalyserWorkspace/PatientsDataware/Xv_Hs__1990_09_03/mra_tridimensionel__image/centerlines/cerebrovascular/R/R000001.txt", vessel);
        this->cuttingLayerOption->setMinimum(0);
        this->cuttingLayerOption->setMaximum(vesselPointCount-1);
    }
    this->flyThroughTimer->start(0.1);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::findPatientExisted
//!
void PatientsWidget::findPatientExisted(){

    numberOfPatient = this->dispatcher->findPatientExisted();

    //!algorithmTestPlatform->setSystemStatus(QString::number(numberOfPatient));

    for(int cpt = numberOfPatient; cpt > 0; cpt--){
        waitingPatientsPhotoStringQueue.append(this->dispatcher->getPatientById(numberOfPatient - cpt)->getPhotoPath());
        waitingPatientsStringNameQueue.append(this->dispatcher->getPatientById(numberOfPatient - cpt)->getName());
        waitingPatientsIDQueue.append(QString::number(numberOfPatient - cpt));
        waitingPatientsMraPathQueue.append(this->dispatcher->getPatientById(numberOfPatient - cpt)->getMraTridimensionelPath());
    }
    //currentPatinetInfoQueue.append(this->dispatcher->getPatientById(numberOfPatient - 1)->getBirthdayOfPatient());
    //currentPatinetInfoQueue.append(this->dispatcher->getPatientById(numberOfPatient - 1)->getSexOfPatient());
    //currentPatinetInfoQueue.append(this->dispatcher->getPatientById(numberOfPatient - 1)->getIdNumber());

    this->displayLastFiveOrLess();

    this->displayCurrentPatinetInfo();

    this->visualizeCurrentVolumeData();

}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::displayLastFiveOrLess
//!
void PatientsWidget::displayLastFiveOrLess(){

    qDebug()<<numberOfPatient;

    int offset = numberOfPatient - 5;

    for(int cpt = 5; cpt > 0; cpt--){
        qDebug()<<waitingPatientsPhotoStringQueue.at(numberOfPatient - cpt);
        waitingPatientsQueue.at(5 - cpt)->setPixmap(waitingPatientsPhotoStringQueue.at(numberOfPatient - cpt));
        waitingPatientsQueue.at(5 - cpt)->setAutoFillBackground(true);
        waitingPatientsQueue.at(5 - cpt)->setScaledContents(true);
        waitingPatientsQueue.at(5 - cpt)->update();

        waitingPatientsNameQueue.at(5 - cpt)->setText(waitingPatientsStringNameQueue.at(numberOfPatient - cpt));
        waitingPatientsNameQueue.at(5 - cpt)->setAutoFillBackground(true);
        waitingPatientsNameQueue.at(5 - cpt)->setScaledContents(true);
        waitingPatientsNameQueue.at(5 - cpt)->update();
    }
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::doLeftSelect
//!
void PatientsWidget::doLeftSelect(){

    QString temp;
    temp = this->waitingPatientsPhotoStringQueue.at(0);
    this->waitingPatientsPhotoStringQueue.pop_front();
    this->waitingPatientsPhotoStringQueue.append(temp);

    QString temp1;
    temp1 = this->waitingPatientsStringNameQueue.at(0);
    this->waitingPatientsStringNameQueue.pop_front();
    this->waitingPatientsStringNameQueue.append(temp1);

    QString temp2;
    temp2 = this->waitingPatientsIDQueue.at(0);
    this->waitingPatientsIDQueue.pop_front();
    this->waitingPatientsIDQueue.append(temp2);

    QString temp3;
    temp3 = this->waitingPatientsMraPathQueue.at(0);
    this->waitingPatientsMraPathQueue.pop_front();
    this->waitingPatientsMraPathQueue.append(temp3);

    this->displayLastFiveOrLess();
    this->displayCurrentPatinetInfo();
    this->visualizeCurrentVolumeData();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::doRightSelect
//!
void PatientsWidget::doRightSelect(){

    //! reorganiser patients queue
    QString temp;
    temp = this->waitingPatientsPhotoStringQueue.at(this->numberOfPatient-1);
    this->waitingPatientsPhotoStringQueue.pop_back();
    this->waitingPatientsPhotoStringQueue.insert(0,temp);

    QString temp1;
    temp1 = this->waitingPatientsStringNameQueue.at(this->numberOfPatient-1);
    this->waitingPatientsStringNameQueue.pop_back();
    this->waitingPatientsStringNameQueue.insert(0,temp1);

    QString temp2;
    temp2 = this->waitingPatientsIDQueue.at(this->numberOfPatient-1);
    this->waitingPatientsIDQueue.pop_back();
    this->waitingPatientsIDQueue.insert(0,temp2);

    QString temp3;
    temp3 = this->waitingPatientsMraPathQueue.at(this->numberOfPatient-1);
    this->waitingPatientsMraPathQueue.pop_back();
    this->waitingPatientsMraPathQueue.insert(0,temp3);


    this->displayLastFiveOrLess();
    this->displayCurrentPatinetInfo();
    this->visualizeCurrentVolumeData();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::displayCurrentPatinetInfo
//!
void PatientsWidget::displayCurrentPatinetInfo(){
    //this->nameLineEdit->setText(waitingPatientsStringNameQueue.at(4));
    //this->birthdayLineEdit->setText(currentPatinetInfoQueue.at(0));
    //this->sexualLineEdit->setText(currentPatinetInfoQueue.at(1));
    //this->idNumberEdit->setText(currentPatinetInfoQueue.at(1));
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::terminateSurgery
//!
void PatientsWidget::terminate(){
    this->surgeryPlanWindow->close();
    this->guidewareTrackingWindow->close();
    this->controlConsoleWindow->close();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::simpleViewButtonClicked
//!
void PatientsWidget::simpleViewButtonClicked(){

    if(!simpleViewButtonIsClicked){
        patientsPhotoWidget->close();
        patientInfoContainer->close();
        analyseResultDisplayArea->close();
        this->simpleModeButton->setIcon(QIcon(":/images/fileList-pressed.png"));
        this->simpleModeButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.027));
    }
    else{
        patientsPhotoWidget->show();
        patientInfoContainer->show();
        analyseResultDisplayArea->show();
        this->simpleModeButton->setIcon(QIcon(":/images/fileList.png"));
        this->simpleModeButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.027));
    }
    simpleViewButtonIsClicked = !simpleViewButtonIsClicked;

}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::doParseCdRom
//!
void PatientsWidget::doParseCdRom(){
    this->algorithmTestPlatform->setSystemStatus("doParseCdRom");
    QFileInfoList drives = QDir::drives();

    for(unsigned char cpt = 0; cpt < drives.size(); cpt++){
        this->algorithmTestPlatform->setSystemStatus(drives.at(cpt).absolutePath());
    }

    dicomCDRomReader->doParseCDRom("D:/");
}


//-------------------------------
//#include<vtkDICOMDirectory.h>
//#include<vtkSmartPointer.h>
//#include<vtkDICOMItem.h>
//#include<vtkDICOMReader.h>
//#include<vtkImageCast.h>
//#include<vtkImageData.h>
//#include<vtkMetaImageWriter.h>
//#include<vtkStringArray.h>
//#include<vtkDICOMDataElement.h>
//#include<vtkDICOMTag.h>
//#include<vtkDICOMDictionary.h>
//#include<vtkDICOMDictEntry.h>
//#include<vtkDICOMMetaData.h>
//#include<io.h>

//std::string ClearSpecialLetter(std::string str)
//{
//    std::string new_str="";
//    for(int i=0;i<str.length();i++)
//    {
//        if((str[i]>='a'&&str[i]<='z')||(str[i]>='A'&&str[i]<='Z')||(str[i]>='0'&&str[i]<='9'))
//            new_str+=str[i];
//    }
//    return new_str;

//}
//-----------------------------------
//    vtkSmartPointer<vtkDICOMDirectory> dicomdir=vtkSmartPointer<vtkDICOMDirectory>::New();
//    dicomdir->SetDirectoryName("E:\\");
//    dicomdir->Update();
////int m=dicomdir->GetNumberOfPatients();
//    int m=dicomdir->GetNumberOfStudies();
//    cout<<m;
//    for (int i=0;i<m;i++)
//    {

//    /*	vtkDICOMItem patient = dicomdir ->GetPatientRecord(i);*/
//        vtkDICOMItem patient = dicomdir ->GetPatientRecordForStudy(i);
//        std::string patientname=ClearSpecialLetter(patient.GetAttributeValue(DC::PatientName).AsUTF8String());
//        //cout<<patientname<<endl;
//        std::string fileboxname="c:\\data\\"+patientname;
//        std::cout<<patientname<<endl;
//        if (_access(fileboxname.c_str(), 0) != 0)
//        {
//            std::string creatcode="mkdir "+fileboxname;
//            system(creatcode.c_str());
//        }
//    /*		for(vtkDICOMDataElementIterator iter=patient.Begin();iter!=patient.End();++iter)
//            {
//                vtkDICOMTag tag=iter->GetTag();
//                vtkDICOMDictEntry entry=vtkDICOMDictionary::FindDictEntry(tag);
//                if(entry.IsValid())
//                {
//                    cout<<entry.GetName();
//                    if (iter -> IsPerInstance ())
//                    {
//                        int n = iter -> GetNumberOfInstances ();
//                        for (int i = 0; i < n; i++)
//                        {
//                            std :: cout <<": " << iter ->GetValue(i) << std :: endl;
//                        }
//                    }
//                    else
//                    {
//                    std :: cout << ":" << iter ->GetValue () << std :: endl;
//                    }
//                }
//            }*/
//        vtkDICOMItem study = dicomdir -> GetStudyRecord (i);
//        int j1 = dicomdir -> GetFirstSeriesForStudy (i);
//        int j2 = dicomdir -> GetLastSeriesForStudy (i);
//        cout<<j1<<endl;
//        cout<<j2<<endl;
//        for (int j = j1; j <= j2; j++)
//        {
//        vtkDICOMItem series = dicomdir -> GetSeriesRecord (j);
//        vtkStringArray *sortedFiles = dicomdir -> GetFileNamesForSeries (j);
//        std::ofstream  filewrite;
//        char numj[256];
//        sprintf(numj,"%d",j);
//        std::string nameaddj=numj;
//        filewrite.open(fileboxname+"\\"+" group_"+numj+"head_data.txt");

//        std::cout<<j<<"----------文件头部信息----------"<<sortedFiles<<endl;
//        filewrite<<j<<"----------文件头部信息----------"<<sortedFiles<<endl;

//        vtkSmartPointer<vtkDICOMReader> reader=vtkSmartPointer<vtkDICOMReader>::New();
//        reader -> SetFileNames (sortedFiles);
//        reader->UpdateInformation();
//        vtkDICOMMetaData *meta=reader->GetMetaData();
//        for(vtkDICOMDataElementIterator iter=meta->Begin();iter!=meta->End();++iter)
//        {
//        vtkDICOMTag tag=iter->GetTag();
//        vtkDICOMDictEntry entry=vtkDICOMDictionary::FindDictEntry(tag);
//            if(entry.IsValid())
//            {
//                cout<<entry.GetName();
//                filewrite<<entry.GetName();
//                if (iter -> IsPerInstance ())
//                {
//                    int n = iter -> GetNumberOfInstances ();
//                    for (int i = 0; i < 1; i++)//	for (int i = 0; i < n; i++)全部输出
//                    {
//                        std :: cout <<": " << iter ->GetValue(i) << std :: endl;
//                        filewrite<<": " << iter ->GetValue(i) <<endl;
//                    }
//                }
//                else
//                {
//                std :: cout << ":" << iter ->GetValue () << std :: endl;
//                filewrite<< ":" << iter ->GetValue () <<endl;
//                }
//            }
//        }
//        filewrite.close();

//        cout<<"ok"<<endl;

//        int TimeDim=reader->GetTimeDimension();
//        cout<<"TimeDimension"<<TimeDim<<endl;

//        int *ext=reader->GetDataExtent();
//        cout<<ext[1]<<" "<<ext[3]<<" "<<ext[5]<<endl;//注释
//        if (ext[5]<2&&TimeDim<2)
//        {
//            cout<<"bad data"<<endl;
//            continue;
//        }
//        reader ->Update ();
//        cout<<"read succeed"<<endl;


//        vtkSmartPointer<vtkImageCast> castFilter = vtkSmartPointer<vtkImageCast>::New();
//        castFilter->SetInputData(reader->GetOutput());
//        castFilter->SetOutputScalarTypeToUnsignedShort();
//        castFilter->Update();
//        cout<<"1"<<endl;

//        vtkSmartPointer<vtkMetaImageWriter> w=vtkSmartPointer<vtkMetaImageWriter>::New();
//        w->SetInputData(castFilter->GetOutput());
//        std::string savepath=fileboxname+"\\"+"group"+"_"+numj+"_IMG.mha";//改
//        w->SetFileName(savepath.c_str());
//        w->Write();
//        cout<<"ok"<<endl;
//        }
//    }
//    system("pause");
//    return 0;

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::addPatient
//!
void PatientsWidget::addPatient(){
    this->patientInformationWidget->display(QCursor::pos());
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::testX
//! \return
//!
int PatientsWidget::testX(){
    return firstPatientPhotoWidget->width();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::testY
//! \return
//!
int PatientsWidget::testY(){
    return firstPatientPhotoWidget->height();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::displayBrainSegImage
//!
void PatientsWidget::visualizeCurrentVolumeData(){

    QString temp = waitingPatientsMraPathQueue.at(numberOfPatient-1);

    QStringList temp1 = temp.split(".");
    if(this->currentModality == "T1"){
        loadMRAImageFile(temp1[0]+"_T1.mhd");

    }
    else if(this->currentModality == "T2"){
        loadMRAImageFile(temp1[0]+"_T2.mhd");

    }
    if(this->currentModality == "TOF"){
        loadMRAImageFile(temp1[0]+"_TOF.mhd");
    }

}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief Patient::loadMRAImageFile
//! \param fileName
//! \return
//!
bool PatientsWidget::loadMRAImageFile(const QString &fileName){

    //! file type check
    eFileType ret =  ImageFileInterface::getFileType(fileName);

    if(ret == UNKOWN_FILE_TYPE){
        return false;
    }

    //! get the instance of related reader according to the file type
    ImageFileInterface *fileInterface = ImageFileInterface::getInstanceFileByType(ret);

    //! read the content of the image
    if(fileInterface->readFrom(fileName) != IMAGE_NO_ERRROR){
        return false;
    }

    //!Pass the reference of the image read to the pointer declared upon
    display(fileInterface->getImage());

    return true;
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::setWorkSpaceColor
//! \param workspaceColor
//!
void PatientsWidget::setWorkSpaceColor(QString workspaceColor){
    if(workspaceColor.contains("rgb")){
        QString color_string = workspaceColor.split("rgb(")[1].split(")")[0];
        QStringList colors = color_string.split(',');
        workspaceRed = colors[0].toInt(0,10);
        workspaceGreen = colors[1].toInt(0,10);
        workspaceBlue = colors[2].toInt(0,10);

    }
    else{
        QColor *qworkspaceColor = new QColor(workspaceColor);

        workspaceRed = qworkspaceColor->red();
        workspaceGreen = qworkspaceColor->green();
        workspaceBlue = qworkspaceColor->blue();
    }


    this->originVolumeDataRenderer->SetBackground((1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255);
    this->flyThroughRenderer->SetBackground((1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255);

    this->setStyleSheet("background-color:"+this->workspaceColor+";color:"+this->globalFontColor);
    this->patientsWidgetToolBar->setStyleSheet("background-color:"+this->workspaceColor);
    this->patientsPhotoWidget->setStyleSheet("background-color:"+this->workspaceColor);
    //this->volumeDataAnalyseArea->setStyleSheet(this->textEditStyleSheet);
    this->patientInfoContainer->setStyleSheet("background-color:"+this->workspaceColor);
}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::display
//! \param imgToBeDisplayed
//!
void PatientsWidget::display(vtkImageData *imgToBeDisplayed){

    this->currentVolumeData = imgToBeDisplayed;

    //! remove
    originVolumeDataRenderer->RemoveAllViewProps();
    flyThroughRenderer->RemoveAllViewProps();

    //! generate shift scale volume data
    double range[2];
    this->currentVolumeData->GetScalarRange(range);
    this->shiftScaleVolumeData->SetShift(-1.0*range[0]);
    this->shiftScaleVolumeData->SetScale(255.0/(range[1] - range[0]));
    this->shiftScaleVolumeData->SetOutputScalarTypeToUnsignedChar();
    this->shiftScaleVolumeData->SetInputData(currentVolumeData);
    this->shiftScaleVolumeData->ReleaseDataFlagOff();
    this->shiftScaleVolumeData->Update();

    //! volume data visualization
    this->volumeMapper->SetInputData(imgToBeDisplayed);
    this->volumeMapper->SetBlendModeToMaximumIntensity();
    this->volume->SetMapper(volumeMapper);


    vtkOutlineFilter *boundbox=vtkOutlineFilter::New();
    boundbox->SetInputData(currentVolumeData);
    boundbox->Update();
    vtkPolyDataMapper *boxmap=vtkPolyDataMapper::New();
    boxmap->SetInputConnection(boundbox->GetOutputPort());
    vtkActor *boxactor=vtkActor::New();
    boxactor->SetMapper(boxmap);
    boxactor->GetProperty()->SetColor(1,1,1);
    boxactor->GetProperty()->SetOpacity(1);



    //this->originVolumeDataRenderer->AddActor(boxactor);
    this->originVolumeDataRenderer->AddVolume(volume);

    this->originVolumeDataRenderer->ResetCamera();

    this->flyThroughRenderer->AddActor(boxactor);
    this->flyThroughRenderer->ResetCamera();

    //! TODO to be verified...
    int imageDims[3];
    currentVolumeData->GetDimensions(imageDims);

    for (int i = 0; i < 3; i++){
      // make them all share the same reslice cursor object.
      vtkResliceCursorLineRepresentation *rep = vtkResliceCursorLineRepresentation::SafeDownCast(riw[i]->GetResliceCursorWidget()->GetRepresentation());
      riw[i]->SetResliceCursor(riw[0]->GetResliceCursor());

      rep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);

      riw[i]->SetInputData(imgToBeDisplayed);
      riw[i]->SetSliceOrientation(i);
      riw[i]->SetResliceModeToAxisAligned();
    }

    vtkRenderWindowInteractor *currentPatientVolumeDataAnalyseAreaInteractor = this->currentPatientVolumeDataAnalyseArea->GetInteractor();
    for (int i = 0; i < 3; i++){
      planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
      planeWidget[i]->SetInteractor(currentPatientVolumeDataAnalyseAreaInteractor);
      planeWidget[i]->SetPicker(picker);
      planeWidget[i]->RestrictPlaneToVolumeOn();
      double color[3] = {0, 0, 0};
      color[i] = 1;
      planeWidget[i]->GetPlaneProperty()->SetColor(color);

      riw[i]->GetRenderer()->SetBackground( (1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255 );

      planeWidget[i]->SetTexturePlaneProperty(ipwProp);
      planeWidget[i]->TextureInterpolateOff();
      planeWidget[i]->SetResliceInterpolateToLinear();
      planeWidget[i]->SetInputData(currentVolumeData);
      planeWidget[i]->SetPlaneOrientation(i);
      planeWidget[i]->SetSliceIndex(imageDims[i]/2);
      planeWidget[i]->DisplayTextOn();
      planeWidget[i]->SetDefaultRenderer(originVolumeDataRenderer);
      planeWidget[i]->SetWindowLevel(1358, -27);
      planeWidget[i]->On();
      planeWidget[i]->InteractionOn();
    }

    vtkSmartPointer<vtkResliceCursorCallback> cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();

    for (int i = 0; i < 3; i++){
      cbk->IPW[i] = planeWidget[i];
      cbk->RCW[i] = riw[i]->GetResliceCursorWidget();
      riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );
      riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::WindowLevelEvent, cbk );
      riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResliceThicknessChangedEvent, cbk );
      riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResetCursorEvent, cbk );
      riw[i]->GetInteractorStyle()->AddObserver(vtkCommand::WindowLevelEvent, cbk );

      // Make them all share the same color map.
      riw[i]->SetLookupTable(riw[0]->GetLookupTable());
      planeWidget[i]->GetColorMap()->SetLookupTable(riw[0]->GetLookupTable());
      //planeWidget[i]->GetColorMap()->SetInput(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap()->GetInput());
      planeWidget[i]->SetColorMap(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap());

    }

    this->ResetViews();

}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::ResetViews
//! \param none
//!
void PatientsWidget::ResetViews()
{
  // Reset the reslice image views
  for (int i = 0; i < 3; i++)
    {
    riw[i]->Reset();
    riw[i]->GetRenderer()->ResetCamera();
    }

  // Also sync the Image plane widget on the 3D top right view with any
  // changes to the reslice cursor.
  for (int i = 0; i < 3; i++)
    {
    vtkPlaneSource *ps = static_cast< vtkPlaneSource * >(
        planeWidget[i]->GetPolyDataAlgorithm());
    ps->SetNormal(riw[0]->GetResliceCursor()->GetPlane(i)->GetNormal());
    ps->SetCenter(riw[0]->GetResliceCursor()->GetPlane(i)->GetOrigin());

    // If the reslice plane has modified, update it on the 3D widget
    this->planeWidget[i]->UpdatePlacement();
    }

  // Render in response to changes.
  for (int i = 0; i < 3; i++)
    {
    riw[i]->Render();
    }
  xzSlice->GetRenderWindow()->Render();
  riw[2]->GetRenderer()->ResetCamera();

}


//! --------------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::update
//!
void PatientsWidget::update(){

    this->patientHandling->doImageFileLecture();

    //! wait lecture finished
    do{
        this->algorithmTestPlatform->setSystemStatus("unfinished");
    }while(!this->patientHandling->readFinished());

    this->visualizeCurrentVolumeData();
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::constructIHM
//!
void PatientsWidget::constructIHM(){

    this->simpleModeButton =  new QPushButton();
    this->simpleModeButton->setIcon(QIcon(":/images/fileList.png"));
    this->simpleModeButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.027));
    this->simpleModeButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.03);
    this->simpleModeButton->setFlat(true);

    this->patientsWidgetConfigurationButton =  new QPushButton();
    this->patientsWidgetConfigurationButton->setIcon(QIcon(":/images/patientWidgetConfiguration.png"));
    this->patientsWidgetConfigurationButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.027));
    this->patientsWidgetConfigurationButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.03);
    this->patientsWidgetConfigurationButton->setFlat(true);

    this->addPatientButton =  new QPushButton();
    this->addPatientButton->setIcon(QIcon(":/images/import_patient.png"));
    this->addPatientButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.027));
    this->addPatientButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.03);
    this->addPatientButton->setFlat(true);

    this->cdRomParseButton =  new QPushButton();
    this->cdRomParseButton->setIcon(QIcon(":/images/Disk.png"));
    this->cdRomParseButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.027));
    this->cdRomParseButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.03);
    this->cdRomParseButton->setFlat(true);

    this->controlBarSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->patientsWidgetToolBar = new QWidget();
    this->patientsWidgetToolBar->setStyleSheet("background-color:"+this->workspaceColor);
    this->patientsWidgetToolBar->setFixedHeight(this->appHeight*0.03);
    this->patientsWidgetToolBarLayout = new QHBoxLayout(this->patientsWidgetToolBar);
    this->patientsWidgetToolBarLayout->addWidget(this->simpleModeButton);
    this->patientsWidgetToolBarLayout->addWidget(this->addPatientButton);
    this->patientsWidgetToolBarLayout->addWidget(this->patientsWidgetConfigurationButton);
    this->patientsWidgetToolBarLayout->addWidget(this->cdRomParseButton);
    this->patientsWidgetToolBarLayout->addSpacerItem(this->controlBarSpacer);
    this->patientsWidgetToolBarLayout->setSpacing(0);
    this->patientsWidgetToolBarLayout->setMargin(0);

    this->downBlackArea = new QWidget();
    this->downBlackArea->setFixedHeight(this->appHeight*0.04);
    this->downBlackArea->setStyleSheet("background-color:transparent;border:0px solid beige");

    this->leftBlackArea = new QWidget();
    this->leftBlackArea->setFixedWidth(this->appWidth*0.05);
    this->leftBlackArea->setStyleSheet("background-color:transparent;border:0px solid beige");

    this->firstPatientPhotoWidget = new PatientItem(appWidth,appHeight,this->caracterStyle, this->globalFontColor);
    this->firstPatientPhotoWidget->setFixedSize(this->appWidth*0.06, this->appHeight*0.22);

    this->secondPatientPhotoWidget = new PatientItem(appWidth,appHeight, this->caracterStyle, this->globalFontColor);
    this->secondPatientPhotoWidget->setFixedSize(this->appWidth*0.06, this->appHeight*0.22);

    this->thirdPatientPhotoWidget = new PatientItem(appWidth,appHeight, this->caracterStyle, this->globalFontColor);
    this->thirdPatientPhotoWidget->setFixedSize(this->appWidth*0.06, this->appHeight*0.22);

    this->fourthPatientPhotoWidget = new PatientItem(appWidth,appHeight, this->caracterStyle, this->globalFontColor);
    this->fourthPatientPhotoWidget->setFixedSize(this->appWidth*0.06, this->appHeight*0.22);

    this->waittingPatientPhotoes = new QWidget();
    this->waittingPatientPhotoes->setStyleSheet("background-color:transparent;border:0px solid beige");
    this->waittingPatientPhotoesLayout = new QHBoxLayout(waittingPatientPhotoes);
    this->waittingPatientPhotoesLayout->addWidget(leftBlackArea);
    this->waittingPatientPhotoesLayout->addWidget(firstPatientPhotoWidget);
    this->waittingPatientPhotoesLayout->addWidget(secondPatientPhotoWidget);
    this->waittingPatientPhotoesLayout->addWidget(thirdPatientPhotoWidget);
    this->waittingPatientPhotoesLayout->addWidget(fourthPatientPhotoWidget);
    this->waittingPatientPhotoesLayout->setSpacing(5);
    this->waittingPatientPhotoesLayout->setMargin(5);

    this->waittingPatientsAndBlackAreaWindow = new QWidget();
    this->waittingPatientsAndBlackAreaWindow->setStyleSheet("background-color:transparent; border:0px solid beige");
    this->waittingPatientsAndBlackAreaWindowLayout = new QVBoxLayout(waittingPatientsAndBlackAreaWindow);
    this->waittingPatientsAndBlackAreaWindowLayout->addWidget(waittingPatientPhotoes);
    this->waittingPatientsAndBlackAreaWindowLayout->addWidget(downBlackArea);
    this->waittingPatientsAndBlackAreaWindowLayout->setSpacing(0);
    this->waittingPatientsAndBlackAreaWindowLayout->setMargin(0);

    //!--------------------------------------------------------------------------------------
    //! current patient widget
    //!--------------------------------------------------------------------------------------

    this->currentPatientInfo = new QLabel();
    this->currentPatientInfo->setStyleSheet("background-color: rgba(0, 0, 0, 100); color:"+globalFontColor);
    this->currentPatientInfo->setFont(*this->caracterStyle);
    this->currentPatientInfo->setFixedSize(this->appWidth*0.08, this->appHeight*0.03);
    this->currentPatientInfo->setAlignment(Qt::AlignCenter);

    this->patientPhotoBlankArea = new QWidget();

    this->currentPatientPhoto = new QLabel();
    this->currentPatientPhoto->setStyleSheet(this->photoLabelStyleSheet);
    this->currentPatientPhoto->setFixedSize(this->appWidth*0.07, this->appHeight*0.21);
    this->currentPatientPhotoLayout = new QVBoxLayout(this->currentPatientPhoto);
    this->currentPatientPhotoLayout->addWidget(this->currentPatientInfo);
    this->currentPatientPhotoLayout->addWidget(this->patientPhotoBlankArea);
    this->currentPatientPhotoLayout->setSpacing(0);
    this->currentPatientPhotoLayout->setMargin(0);

    this->surgeryLaunchButton = new QPushButton();
    this->surgeryLaunchButton->setText("launch");
    this->surgeryLaunchButton->setFont(QFont("Helvetica",13,QFont::DemiBold, false));
    this->surgeryLaunchButton->setStyleSheet("background-color:"+this->globalFontColor+";border:0px solid lightgray;color:black");
    this->surgeryLaunchButton->setFixedSize(this->appWidth*0.07, this->appHeight*0.05);

    this->patientHandlingWidget = new QWidget();
    this->patientHandlingWidget->setStyleSheet("background-color:transparent; border:0px solid darkgray; ");
    this->patientHandlingWidget->setFixedSize(this->appWidth*0.07, this->appHeight*0.26);
    this->patientHandlingWidgetLayout = new QVBoxLayout(patientHandlingWidget);
    this->patientHandlingWidgetLayout->addWidget(currentPatientPhoto);
    this->patientHandlingWidgetLayout->addWidget(surgeryLaunchButton);
    this->patientHandlingWidgetLayout->setSpacing(0);
    this->patientHandlingWidgetLayout->setMargin(0);

    this->waitingPatientsQueue.append(this->firstPatientPhotoWidget->getPatientPhoto());
    this->waitingPatientsQueue.append(this->secondPatientPhotoWidget->getPatientPhoto());
    this->waitingPatientsQueue.append(this->thirdPatientPhotoWidget->getPatientPhoto());
    this->waitingPatientsQueue.append(this->fourthPatientPhotoWidget->getPatientPhoto());
    this->waitingPatientsQueue.append(this->currentPatientPhoto);

    this->waitingPatientsNameQueue.append(this->firstPatientPhotoWidget->getPatientName());
    this->waitingPatientsNameQueue.append(this->secondPatientPhotoWidget->getPatientName());
    this->waitingPatientsNameQueue.append(this->thirdPatientPhotoWidget->getPatientName());
    this->waitingPatientsNameQueue.append(this->fourthPatientPhotoWidget->getPatientName());
    this->waitingPatientsNameQueue.append(this->currentPatientInfo);

    //!--------------------------------------------------------------------------------------
    //!the selection buttons
    //!--------------------------------------------------------------------------------------
    this->leftSelectButton = new QPushButton();
    this->leftSelectButton->setFixedSize(this->appWidth*0.015,this->appHeight*0.26);
    this->leftSelectButton->setIcon(QIcon(":/images/splitter_left.png"));
    this->leftSelectButton->setStyleSheet("background-color:transparent; border:0px solid darkgray; ");
    this->leftSelectButton->setFlat(true);

    this->rightSelectButton = new QPushButton();
    this->rightSelectButton->setFixedSize(this->appWidth*0.015,this->appHeight*0.26);
    this->rightSelectButton->setIcon(QIcon(":/images/splitter_right.png"));
    this->rightSelectButton->setStyleSheet("background-color:transparent; border:0px solid darkgray;");
    this->rightSelectButton->setFlat(true);

    //!--------------------------------------------------------------------------------------
    //!patient photo widget: there are 3 photoes of waitting patients and 1 current patient
    //!--------------------------------------------------------------------------------------
    this->patientsPhotoWidget = new QLabel();
    this->patientsPhotoWidget->setFixedSize(this->appWidth*0.4, this->appHeight*0.26);
    this->patientsPhotoWidgetLayout = new QHBoxLayout(this->patientsPhotoWidget);
    this->patientsPhotoWidgetLayout->addWidget(waittingPatientsAndBlackAreaWindow);
    this->patientsPhotoWidgetLayout->addWidget(leftSelectButton);
    this->patientsPhotoWidgetLayout->addWidget(patientHandlingWidget);
    this->patientsPhotoWidgetLayout->addWidget(rightSelectButton);
    this->patientsPhotoWidgetLayout->setSpacing(0);
    this->patientsPhotoWidgetLayout->setMargin(0);

    //!--------------------------------------------------------------------------------------
    //!the information of the current patient
    //!--------------------------------------------------------------------------------------
    this->nameLabel = new QLabel("name: ");
    this->nameLabel->setFixedSize(appWidth*0.04, appHeight*0.03);
    this->nameLabel->setFont(*this->caracterStyle);
    this->nameLabel->setStyleSheet(this->labelStyleSheet);

    this->birthdayLabel = new QLabel("birth: ");
    this->birthdayLabel->setFixedSize(appWidth*0.04, appHeight*0.03);
    this->birthdayLabel->setFont(*this->caracterStyle);
    this->birthdayLabel->setStyleSheet(this->labelStyleSheet);

    this->sexualLabel = new QLabel("sex: ");
    this->sexualLabel->setFixedSize(appWidth*0.04, appHeight*0.03);
    this->sexualLabel->setFont(*this->caracterStyle);
    this->sexualLabel->setStyleSheet(this->labelStyleSheet);

    this->ageLabel = new QLabel("age: ");
    this->ageLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->ageLabel->setFont(*this->caracterStyle);
    this->ageLabel->setStyleSheet(this->labelStyleSheet);

    this->idNumberLabel = new QLabel("number: ");
    this->idNumberLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->idNumberLabel->setFont(*this->caracterStyle);
    this->idNumberLabel->setStyleSheet(this->labelStyleSheet);

    this->marryLabel = new QLabel("marriage: ");
    this->marryLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->marryLabel->setFont(*this->caracterStyle);
    this->marryLabel->setStyleSheet(this->labelStyleSheet);

    this->nationLabel = new QLabel("nationality: ");
    this->nationLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->nationLabel->setFont(*this->caracterStyle);
    this->nationLabel->setStyleSheet(this->labelStyleSheet);

    this->professionalLabel = new QLabel("professionnel: ");
    this->professionalLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->professionalLabel->setFont(*this->caracterStyle);
    this->professionalLabel->setStyleSheet(this->labelStyleSheet);

    this->leadDoctorLabel = new QLabel("doctor: ");
    this->leadDoctorLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->leadDoctorLabel->setFont(*this->caracterStyle);
    this->leadDoctorLabel->setStyleSheet(this->labelStyleSheet);

    this->therapyTimeLabel = new QLabel("therapy date: ");
    this->therapyTimeLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->therapyTimeLabel->setFont(*this->caracterStyle);
    this->therapyTimeLabel->setStyleSheet(this->labelStyleSheet);

    this->drugAllergyLabel = new QLabel("drug allergy: ");
    this->drugAllergyLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->drugAllergyLabel->setFont(*this->caracterStyle);
    this->drugAllergyLabel->setStyleSheet(this->labelStyleSheet);

    this->remarksLabel = new QLabel("ps: ");
    this->remarksLabel->setFixedSize(appWidth*0.04,appHeight*0.03);
    this->remarksLabel->setFont(*this->caracterStyle);
    this->remarksLabel->setStyleSheet(this->labelStyleSheet);

    this->nameLineEdit = new QLineEdit();
    this->nameLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->nameLineEdit->setFont(*this->caracterStyle);
    this->nameLineEdit->setStyleSheet(this->labelStyleSheet);

    this->birthdayLineEdit = new QLineEdit();
    this->birthdayLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->birthdayLineEdit->setFont(*this->caracterStyle);
    this->birthdayLineEdit->setStyleSheet(this->labelStyleSheet);

    this->sexualLineEdit = new QLineEdit();
    this->sexualLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->sexualLineEdit->setFont(*this->caracterStyle);
    this->sexualLineEdit->setStyleSheet(this->labelStyleSheet);

    this->ageLineEdit = new QLineEdit();
    this->ageLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->ageLineEdit->setFont(*this->caracterStyle);
    this->ageLineEdit->setStyleSheet(this->labelStyleSheet);

    this->idNumberEdit = new QLineEdit();
    this->idNumberEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->idNumberEdit->setFont(*this->caracterStyle);
    this->idNumberEdit->setStyleSheet(this->labelStyleSheet);

    this->marryLineEdit = new QLineEdit();
    this->marryLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->marryLineEdit->setFont(*this->caracterStyle);
    this->marryLineEdit->setStyleSheet(this->labelStyleSheet);

    this->nationLineEdit = new QLineEdit();
    this->nationLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->nationLineEdit->setFont(*this->caracterStyle);
    this->nationLineEdit->setStyleSheet(this->labelStyleSheet);

    this->professionalLineEdit = new QLineEdit();
    this->professionalLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->professionalLineEdit->setFont(*this->caracterStyle);
    this->professionalLineEdit->setStyleSheet(this->labelStyleSheet);

    this->leadDoctorEdit = new QLineEdit();
    this->leadDoctorEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->leadDoctorEdit->setFont(*this->caracterStyle);
    this->leadDoctorEdit->setStyleSheet(this->labelStyleSheet);

    this->therapyTimeEdit = new QLineEdit();
    this->therapyTimeEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->therapyTimeEdit->setFont(*this->caracterStyle);
    this->therapyTimeEdit->setStyleSheet(this->labelStyleSheet);

    this->drugAllergyLineEdit = new QLineEdit();
    this->drugAllergyLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->drugAllergyLineEdit->setFont(*this->caracterStyle);
    this->drugAllergyLineEdit->setStyleSheet(this->labelStyleSheet);

    this->remarksLineEdit = new QLineEdit();
    this->remarksLineEdit->setFixedSize(appWidth*0.1, appHeight*0.03);
    this->remarksLineEdit->setFont(*this->caracterStyle);
    this->remarksLineEdit->setStyleSheet(this->labelStyleSheet);

    this->patientInfoContainer = new QLabel();
    this->patientInfoContainer->setStyleSheet("background-color:red");
    this->patientInfoContainer->setFixedSize(this->appWidth*0.28, appHeight*0.26);
    this->patientInfoContainerLayout = new QGridLayout(patientInfoContainer);
    this->patientInfoContainerLayout->addWidget(nameLabel, 0, 0);
    this->patientInfoContainerLayout->addWidget(nameLineEdit, 0, 1);
    this->patientInfoContainerLayout->addWidget(birthdayLabel, 0, 2);
    this->patientInfoContainerLayout->addWidget(birthdayLineEdit, 0, 3);
    this->patientInfoContainerLayout->addWidget(sexualLabel, 1, 0);
    this->patientInfoContainerLayout->addWidget(sexualLineEdit, 1, 1);
    this->patientInfoContainerLayout->addWidget(ageLabel, 1, 2);
    this->patientInfoContainerLayout->addWidget(ageLineEdit, 1, 3);
    this->patientInfoContainerLayout->addWidget(idNumberLabel, 2, 0);
    this->patientInfoContainerLayout->addWidget(idNumberEdit, 2, 1);
    this->patientInfoContainerLayout->addWidget(marryLabel, 2, 2);
    this->patientInfoContainerLayout->addWidget(marryLineEdit, 2, 3);
    this->patientInfoContainerLayout->addWidget(nationLabel, 3, 0);
    this->patientInfoContainerLayout->addWidget(nationLineEdit, 3, 1);
    this->patientInfoContainerLayout->addWidget(professionalLabel, 3, 2);
    this->patientInfoContainerLayout->addWidget(professionalLineEdit, 3, 3);
    this->patientInfoContainerLayout->addWidget(leadDoctorLabel, 4, 0);
    this->patientInfoContainerLayout->addWidget(leadDoctorEdit, 4, 1);
    this->patientInfoContainerLayout->addWidget(therapyTimeLabel, 4, 2);
    this->patientInfoContainerLayout->addWidget(therapyTimeEdit, 4, 3);
    this->patientInfoContainerLayout->addWidget(drugAllergyLabel, 5, 0);
    this->patientInfoContainerLayout->addWidget(drugAllergyLineEdit, 5, 1);
    this->patientInfoContainerLayout->addWidget(remarksLabel, 5, 2);
    this->patientInfoContainerLayout->addWidget(remarksLineEdit, 5, 3);
    this->patientInfoContainerLayout->setSpacing(0);
    this->patientInfoContainerLayout->setMargin(0);

    //!--------------------------------------------------------------------------------------
    //! Patient's mri image display area
    //!--------------------------------------------------------------------------------------
    this->imageConfigurationAreaSpacer =  new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->guidewareMovementButton =  new QPushButton();
    this->guidewareMovementButton->setIcon(QIcon(":/images/title.png"));
    this->guidewareMovementButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.025));
    this->guidewareMovementButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.025);
    this->guidewareMovementButton->setFlat(true);

    this->cameraFlyThroughButton =  new QPushButton();
    this->cameraFlyThroughButton->setIcon(QIcon(":/images/camera.png"));
    this->cameraFlyThroughButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.025));
    this->cameraFlyThroughButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.025);
    this->cameraFlyThroughButton->setFlat(true);

    this->clearCenterLineButton =  new QPushButton();
    this->clearCenterLineButton->setIcon(QIcon(":/images/clear.png"));
    this->clearCenterLineButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.025));
    this->clearCenterLineButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.025);
    this->clearCenterLineButton->setFlat(true);

    this->autoVolumeRenderingButton =  new QPushButton();
    this->autoVolumeRenderingButton->setIcon(QIcon(":/images/render.png"));
    this->autoVolumeRenderingButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.025));
    this->autoVolumeRenderingButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.025);
    this->autoVolumeRenderingButton->setFlat(true);

    this->threeSliceEnableButton =  new QPushButton();
    this->threeSliceEnableButton->setIcon(QIcon(":/images/three_slice.png"));
    this->threeSliceEnableButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.025));
    this->threeSliceEnableButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.025);
    this->threeSliceEnableButton->setFlat(true);

    this->tissueExtractionButton =  new QPushButton();
    this->tissueExtractionButton->setIcon(QIcon(":/images/extract.png"));
    this->tissueExtractionButton->setIconSize(QSize(this->appWidth*0.015,this->appHeight*0.025));
    this->tissueExtractionButton->setFixedSize(this->appWidth*0.015, this->appHeight*0.025);
    this->tissueExtractionButton->setFlat(true);

    this->vesselModelingLineEdit = new QLineEdit();
    this->vesselModelingLineEdit->setFixedSize(this->appWidth*0.15, this->appHeight*0.025);
    this->vesselModelingLineEdit->setFont(QFont("Courier",9,QFont::AnyStyle, false));
    this->vesselModelingLineEdit->setStyleSheet("border: 1px solid darkgray ;border-radius: 0px;background-color: transparent; color: beige");
    this->vesselModelingLineEdit->setText("add point p0 at (10.1,9.2,12.3,2.6)");

    this->modalityOptionComboBox = new QComboBox();
    this->modalityOptionComboBox->setFixedSize(this->appWidth*0.05, this->appHeight*0.025);
    this->modalityOptionComboBox->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, false));
    this->modalityOptionComboBox->setStyleSheet("QComboBox { border: 1px solid darkgray;  } "
                                              "QComboBox:editable { background-color: rgb(58,89,92); color: darkSeaGreen } ");

    this->modalityOptionComboBox->addItem("T1");
    this->modalityOptionComboBox->addItem("T2");
    this->modalityOptionComboBox->addItem("TOF");

    this->imageConfigurationArea = new QLabel();
    this->imageConfigurationArea->setStyleSheet("background:"+this->workspaceColor);
    this->imageConfigurationArea->setFixedSize(this->appWidth*0.4, this->appHeight*0.04);
    this->imageConfigurationAreaLayout = new QHBoxLayout(this->imageConfigurationArea);
    this->imageConfigurationAreaLayout->addWidget(this->guidewareMovementButton);
    this->imageConfigurationAreaLayout->addWidget(this->cameraFlyThroughButton);
    this->imageConfigurationAreaLayout->addWidget(this->clearCenterLineButton);
    this->imageConfigurationAreaLayout->addWidget(this->autoVolumeRenderingButton);
    this->imageConfigurationAreaLayout->addWidget(this->threeSliceEnableButton);
    this->imageConfigurationAreaLayout->addWidget(this->tissueExtractionButton);
    this->imageConfigurationAreaLayout->addWidget(this->vesselModelingLineEdit);
    this->imageConfigurationAreaLayout->addItem(this->imageConfigurationAreaSpacer);
    this->imageConfigurationAreaLayout->addWidget(this->modalityOptionComboBox);
    this->imageConfigurationAreaLayout->setSpacing(0);
    this->imageConfigurationAreaLayout->setMargin(0);

    this->currentPatientVolumeDataAnalyseArea = new QVTKWidget();
    //this->currentPatientVolumeDataAnalyseArea->setFixedSize(this->appWidth*0.4, this->appHeight*0.68);
    this->currentPatientVolumeDataAnalyseArea->SetRenderWindow(renderWindow);

    this->patientImageDispalyArea = new QLabel();
    //this->patientImageDispalyArea->setFixedSize(this->appWidth*0.4, this->appHeight*0.72);
    this->patientImageDispalyAreaLayout = new QVBoxLayout(patientImageDispalyArea);
    this->patientImageDispalyAreaLayout->addWidget(this->currentPatientVolumeDataAnalyseArea);
    this->patientImageDispalyAreaLayout->addWidget(this->imageConfigurationArea);
    this->patientImageDispalyAreaLayout->setSpacing(0);
    this->patientImageDispalyAreaLayout->setMargin(0);

    //! ------------------------------------------------------------------------------------------------------------------
    //!
    //! interventional route planning
    //!
    this->interventionalRoutePlanningArea = new QVTKWidget();
    //this->interventionalRoutePlanningArea->setFixedSize(this->appWidth*0.27, this->appHeight*0.68);
    this->interventionalRoutePlanningArea->SetRenderWindow(flyThroughRenderWindow);

    this->flyThroughtConfigurationBarSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->cutButton =  new QPushButton();
    this->cutButton->setIcon(QIcon(":/images/couteau.png"));
    this->cutButton->setIconSize(QSize(this->appWidth*0.02,this->appHeight*0.03));
    this->cutButton->setFixedSize(this->appWidth*0.02, this->appHeight*0.03);
    this->cutButton->setFlat(true);

    this->cuttingLayerOption = new QSpinBox();
    this->cuttingLayerOption->setFixedSize(this->appWidth*0.03, this->appHeight*0.03);
    this->cuttingLayerOption->setStyleSheet("QSpinBox {background:"+this->workspaceColor+";border: 1 solide darkgray; color:beige;;selection-background-color: black;}"
                                            "QSpinBox::up-arrow {image: url(:/images/up_arrow.png);width: 5px;height: 5px;}"
                                            "QSpinBox::down-arrow {image: url(:/images/down_arrow.png);width: 5px;height: 5px; }");

    this->reconstructButton =  new QPushButton();
    this->reconstructButton->setIcon(QIcon(":/images/play.png"));
    this->reconstructButton->setIconSize(QSize(this->appWidth*0.02,this->appHeight*0.03));
    this->reconstructButton->setFixedSize(this->appWidth*0.02, this->appHeight*0.03);
    this->reconstructButton->setFlat(true);

    this->flyThroughtConfigurationBar = new QLabel();
    this->flyThroughtConfigurationBar->setFixedSize(this->appWidth*0.27, this->appHeight*0.04);
    this->flyThroughtConfigurationBarLayout = new QHBoxLayout(this->flyThroughtConfigurationBar);
    this->flyThroughtConfigurationBarLayout->addWidget(this->cutButton);
    this->flyThroughtConfigurationBarLayout->addWidget(this->cuttingLayerOption);
    this->flyThroughtConfigurationBarLayout->addWidget(this->reconstructButton);
    this->flyThroughtConfigurationBarLayout->addItem(this->flyThroughtConfigurationBarSpacer);
    this->flyThroughtConfigurationBarLayout->setSpacing(0);
    this->flyThroughtConfigurationBarLayout->setMargin(0);

    this->surgeryPlanArea = new QLabel();
    //this->surgeryPlanArea->setFixedSize(this->appWidth*0.27, this->appHeight*0.72);
    this->surgeryPlanAreaLayout = new QVBoxLayout(this->surgeryPlanArea);
    this->surgeryPlanAreaLayout->addWidget(interventionalRoutePlanningArea);
    this->surgeryPlanAreaLayout->addWidget(flyThroughtConfigurationBar);
    this->surgeryPlanAreaLayout->setSpacing(0);
    this->surgeryPlanAreaLayout->setMargin(0);


    //! xy, yz, xz slice image
    xySlice = new QVTKWidget();
    xySlice->setFixedSize(this->appWidth*0.32, this->appHeight*0.32);

    yzSlice = new QVTKWidget();
    yzSlice->setFixedSize(this->appWidth*0.32, this->appHeight*0.32);

    xzSlice = new QVTKWidget();
    xzSlice->setFixedSize(this->appWidth*0.32, this->appHeight*0.32);

    //! TODO naming...
    this->xySlice->SetRenderWindow(riw[0]->GetRenderWindow());
    this->riw[0]->SetupInteractor(this->xySlice->GetRenderWindow()->GetInteractor());

    this->yzSlice->SetRenderWindow(riw[1]->GetRenderWindow());
    this->riw[1]->SetupInteractor(this->yzSlice->GetRenderWindow()->GetInteractor());

    this->xzSlice->SetRenderWindow(riw[2]->GetRenderWindow());
    this->riw[2]->SetupInteractor(this->xzSlice->GetRenderWindow()->GetInteractor());

    slicingConfigurationBar = new QLabel();
    slicingConfigurationBar->setFixedSize(this->appWidth*0.32, this->appHeight*0.04);

    analyseResultDisplayArea = new QLabel();
    analyseResultDisplayArea->setFixedWidth(this->appWidth*0.32);
    analyseResultDisplayAreaLayout = new QVBoxLayout(analyseResultDisplayArea);

    analyseResultDisplayAreaLayout->addWidget(xySlice);
    analyseResultDisplayAreaLayout->addWidget(yzSlice);
    analyseResultDisplayAreaLayout->addWidget(xzSlice);
    analyseResultDisplayAreaLayout->addWidget(slicingConfigurationBar);

    analyseResultDisplayAreaLayout->setSpacing(0);
    analyseResultDisplayAreaLayout->setMargin(0);

    //! --------------------------------------------------------------------------------------
    //! patients widget workspace
    //! --------------------------------------------------------------------------------------
    this->patientsWidgetWorkspace = new QWidget();
    this->patientsWidgetWorkspaceLayout = new QGridLayout(this->patientsWidgetWorkspace);
    this->patientsWidgetWorkspaceLayout->addWidget(patientsPhotoWidget, 0, 0);
    this->patientsWidgetWorkspaceLayout->addWidget(patientInfoContainer,0, 1);
    this->patientsWidgetWorkspaceLayout->addWidget(patientImageDispalyArea, 1, 0);
    this->patientsWidgetWorkspaceLayout->addWidget(surgeryPlanArea, 1, 1);
    this->patientsWidgetWorkspaceLayout->setSpacing(0);
    this->patientsWidgetWorkspaceLayout->setMargin(0);

    this->medicalImageAnalyseArea = new QWidget();
    this->medicalImageAnalyseAreaLayout = new QHBoxLayout(medicalImageAnalyseArea);
    this->medicalImageAnalyseAreaLayout->addWidget(patientsWidgetWorkspace);
    this->medicalImageAnalyseAreaLayout->addWidget(analyseResultDisplayArea);
    this->medicalImageAnalyseAreaLayout->setSpacing(0);
    this->medicalImageAnalyseAreaLayout->setMargin(0);

    //!--------------------------------------------------------------------------------------
    //!the layout of patients widget
    //!--------------------------------------------------------------------------------------
    patientsWidgetLayout = new QVBoxLayout(this);
    patientsWidgetLayout->addWidget(this->patientsWidgetToolBar);
    patientsWidgetLayout->addWidget(this->medicalImageAnalyseArea);
    patientsWidgetLayout->setSpacing(0);
    patientsWidgetLayout->setContentsMargins(0,0,0,0);
}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::addPatientToListWidget
//! \param patients
//!
void PatientsWidget::addPatientToWidget(QString path){
    QListWidgetItem *patientItem = new QListWidgetItem();
    patientItem->setIcon(QIcon(path));

}

//!----------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::keyPressEvent
//! \param event
//!
 void PatientsWidget::keyPressEvent(QKeyEvent *event){
     switch(event->key()){
        case Qt::Key_Left:
            this->doLeftSelect();
            break;
        case Qt::Key_Right:
            this->doRightSelect();
            break;
     }
 }

 //!----------------------------------------------------------------------------------------------------
 //!
 //! \brief PatientsWidget::drawBackground
 //! \param widget
 //! \param path
 //!
 void PatientsWidget::drawBackground(QWidget *widget, QString path, int w, int h){

     QPixmap *pixmap = new QPixmap(path);
     QPalette palette = widget->palette();

     palette.setBrush(QPalette::Background, QBrush(pixmap->scaled(QSize(w, h),
                                                                  Qt::IgnoreAspectRatio,
                                                                  Qt::SmoothTransformation)));
     widget->setPalette(palette);
     widget->setMask(pixmap->mask());
 }

 //! ---------------------------------------------------------------------------------------------------
 //!
 //! \brief PatientsWidget::setPatientHandling
 //! \param patientHandling
 //!
 void PatientsWidget::setPatientHandling(Patient *patientHandling){
     this->patientHandling = patientHandling;
 }
