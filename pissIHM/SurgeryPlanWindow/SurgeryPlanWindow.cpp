#include "SurgeryPlanWindow.h"


/**
 * @brief SurgeryPlanWindow::SurgeryPlanWindow
 * @param rect
 * @param surgeryTime
 * @param systemDispatcher
 */
SurgeryPlanWindow::SurgeryPlanWindow(QRect rect,
                                     QTime* surgeryTime,
                                     SystemDispatcher* systemDispatcher,
                                     QString globalWorkSpaceColor,
                                     QString configuratonFilePath) : QWidget()
{  
    //! attributes assignment
    this->rect = rect;
    this->appWidth = rect.width();
    this->appHeight = rect.height();
    this->systemDispatcher = systemDispatcher;
    this->surgeryTime = surgeryTime;
    this->globalWorkSpaceColor = globalWorkSpaceColor;
    this->configuratonFilePath = configuratonFilePath;

    this->initialisation();
    this->constructIHM();
    this->setConnections();
    this->drawBackground();
    this->configuration();
    //this->displayImageAnalyseArea();

}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::updateNDIPos
//!
void SurgeryPlanWindow::updateNDIPos(){
//    if(previousNDIPosActor != NULL){
//        this->mainRenderer->RemoveActor(previousNDIPosActor);
//    }
    
    igtNDIPosition *ret = this->patientHandling->fetchNDIPosition();
    if(ret == NULL){
        return;
    }
    
    //positionList.append(ret);

    //! TODO .....
    vtkSphereSource *pos = vtkSphereSource::New();
    pos->SetRadius(1);
    pos->SetThetaResolution(12);
    pos->SetPhiResolution(6);
    pos->SetCenter(ret->getPositionX(),ret->getPositionY(),ret->getPositionZ());

    this->patientHandling->removeFirstNDIPosition();

    vtkPolyDataMapper *posMapper = vtkPolyDataMapper::New();
    posMapper->SetInputConnection(pos->GetOutputPort());

    vtkActor *posActor = vtkActor::New();
    posActor->SetMapper(posMapper);
    posActor->GetProperty()->SetColor(0,169,0);

    previousNDIPosActor = posActor;

    this->mainRenderer->AddActor(posActor);

    this->patientMRAImage->update();

    qDebug()<<ret->getTimestamps()<<ret->getPositionX()<<ret->getPositionY()<<ret->getPositionZ();
    QString x = QString::number(ret->getTimestamps())+";"+  QString::number(ret->getPositionX())+";"+ QString::number(ret->getPositionY())+";"+  QString::number(ret->getPositionZ());
    ndiInfo.append(x);

    if(ndiInfo.length() == 100){
        this->documenter();
        ndiInfo.clear();
        this->ndiInfoCount += 1;
    }
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::documenter
//!
void SurgeryPlanWindow::documenter(){


    QFile f(this->patientHandling->getMagneticTackingDataPath()+QString::number(this->ndiInfoCount)+".csv");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        cout << "Open failed." << endl;
    }

    QTextStream txtOutput(&f);

    for(int cpt = 0; cpt < ndiInfo.length(); cpt++){
        txtOutput <<ndiInfo.at(cpt)<< endl;
    }

    f.close();
}

//! ----------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::reconstructButtonClicked
//!
void SurgeryPlanWindow::reconstructButtonClicked(){
    QList<QString> filenames = this->patientHandling->getCenterLines();

    for(int i = 0; i < filenames.length(); i++){
        QVector<vtkActor*> interventionalPair = this->constructEachBranchByCoordinatesAndRadius(&this->patientHandling->getVesselByName(filenames.at(i)));
        this->mainRenderer->AddActor(interventionalPair.at(0));
        this->mainRenderer->AddActor(interventionalPair.at(1));
        vesselActors[filenames.at(i)] = interventionalPair.at(0);
        cheminActors[filenames.at(i)] = interventionalPair.at(1);
    }

    this->patientMRAImage->update();
}

//! ------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientsWidget::constructEachBranchByCoordinatesAndRadius
//! \param fileName
//! \return
//!
QVector<vtkActor*> SurgeryPlanWindow::constructEachBranchByCoordinatesAndRadius( QVector<CenterLinePoint*>* vesselPoints){

    vtkPoints* interventionalCheminPoints = vtkPoints::New();
    vtkCellArray* vReconstructLines = vtkCellArray::New();
    vtkPolyData* vReconstructPolyData = vtkPolyData::New();
    vtkDoubleArray* vReconstructRadius = vtkDoubleArray::New();
    vtkTubeFilter* vReconstructTube = vtkTubeFilter::New();
    vtkPolyDataMapper* vReconstructMapper = vtkPolyDataMapper::New();
    vtkActor* vesselTubeActor = vtkActor::New();

    int cpt;
    for(cpt = 0; cpt<vesselPoints->size(); cpt++){
        interventionalCheminPoints->InsertPoint(cpt,vesselPoints->at(cpt)->get_abscissa()-33,vesselPoints->at(cpt)->get_ordinate()-6,vesselPoints->at(cpt)->get_isometric()+120.805);
    }

    vReconstructLines->InsertNextCell(vesselPoints->size());

    for(cpt = 0; cpt<vesselPoints->size(); cpt++){
        vReconstructLines->InsertCellPoint(cpt);
    }

    vReconstructPolyData->SetPoints(interventionalCheminPoints);
    vReconstructPolyData->SetLines(vReconstructLines);

    vReconstructRadius->SetName("radius");
    vReconstructRadius->SetNumberOfTuples(vesselPoints->size());
    for(cpt = 0; cpt<vesselPoints->size(); cpt++){
        if(vesselPoints->at(cpt)->get_radius()==NULL){
            vReconstructRadius->SetTuple1(cpt,1);
        }else{
            vReconstructRadius->SetTuple1(cpt,vesselPoints->at(cpt)->get_radius());
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
    vesselTubeActor->GetProperty()->SetOpacity(0.8);
    vesselTubeActor->GetProperty()->SetColor(250.0/255,69.0/255,0.0/255);

    QVector<vtkActor*> interventionalPair;
    interventionalPair.append(vesselTubeActor);
    interventionalPair.append(this->visualiserInterventionalChemin(interventionalCheminPoints));

    return interventionalPair;
}

//! ----------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::visualiserInterventionalChemin
//! \param vReconstructPoints
//! \return
//!
vtkActor* SurgeryPlanWindow::visualiserInterventionalChemin(vtkPoints* vReconstructPoints){
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


//! ------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::displayTragetArchitecture
//!
void SurgeryPlanWindow::displayTragetArchitecture(){
    this->mainRenderer->RemoveAllViewProps();
    QString target_stl = this->patientHandling->getTridimensionelPath()+"vascular/phantom.stl";
    vtkSTLReader* stlReader = vtkSTLReader::New();;
    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    vtkActor* actor = vtkActor::New();

    stlReader->SetFileName(target_stl.toLocal8Bit().data());
    stlReader->Update();

    mapper->SetInputConnection(stlReader->GetOutputPort());
    actor->SetMapper(mapper);
    actor->GetProperty()->SetOpacity(0.65);

    //this->mainRenderer->AddActor(actor);

    this->patientMRAImage->update();
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::setWorkSpaceColor
//! \param workspaceColor
//!
void SurgeryPlanWindow::setWorkSpaceColor(QString workspaceColor){

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

    this->histogramPlottingBoard->setBackgroundColor(workspaceRed, workspaceGreen, workspaceBlue);
    this->transformationPlottingBoard->setBackgroundColor(workspaceRed, workspaceGreen, workspaceBlue);

    this->cprRenderer->SetBackground((1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255);
    this->mainRenderer->SetBackground((1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255);
    this->singleVesselRenderer->SetBackground((1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255);
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::setStartTime
//! \param start_time
//!
void SurgeryPlanWindow::setStartTime(int start_time){
    this->start_time = start_time;
    //this->timer->start(1000);
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::showTime
//!
void SurgeryPlanWindow::toFindReconstructedResult(){
    if(this->patientHandling->doReadReconstructedResult()){

        this->displayPatientMRAImage();
        this->updatePatientMRAImageStatistics();
        this->updatePatientMRAImageHistogram();
        this->initialRendering();
        this->loadVesselsExisted();
        this->timerToWaittingReconstructedResult->stop();

        //! -----------------------------------------------------------------------
        QString collaborativeState = "on"; //on/off
        QString collaborativeName = this->patientHandling->getName();
        QString collaborativeType = "reconstructed"; //standby/normal/reconstruct/reconstructed
        QString collaborativePath = this->patientHandling->getCTImagePath() + "reconstruct";

        QFile f(this->configuratonFilePath);
        if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            cout << "Open failed." << endl;
        }

        QTextStream txtOutput(&f);
        QString s1(collaborativeState);
        QString s2(collaborativeName);
        QString s3(collaborativeType);
        QString s4(collaborativePath);

        txtOutput << "state:" << s1 << endl;
        txtOutput << "name:" << s2 << endl;
        txtOutput << "type:" << s3 << endl;
        txtOutput << "path:" << s4 << endl;

        f.close();
        //! -----------------------------------------------------------------------
    }
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::update
//!
void SurgeryPlanWindow::update(){

    //this->timerToWaittingReconstructedResult->start(1000);

    this->patientHandling->doImageFileLecture();

    //! wait lecture finished
    do{

    }while(!this->patientHandling->readFinished());

    this->updatePatientPersonelInformation();

    this->displayPatientMRAImage();
    this->updatePatientMRAImageStatistics();
    this->updatePatientMRAImageHistogram();
    this->initialRendering();

    //this->loadVesselsExisted();

    this->realTimeTrackingNDITimer->start(100);
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::loadVesselsExisted
//!
void SurgeryPlanWindow::loadVesselsExisted(){
//    QString path = this->patientHandling->getCenterLineFolderPath();
//    QDir recoredDir(path);
//    QStringList allFiles = recoredDir.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);//(QDir::Filter::Files,QDir::SortFlag::NoSort)

//    if(allFiles.size()>0){
//        for(int i=0 ; i<allFiles.size(); i++){
//            QTreeWidgetItem *vesselItem = new QTreeWidgetItem(vesselsFolder,QStringList(QString(allFiles.at(i))));
//            vesselItem->setIcon(0, *this->fileUnloadedIcon);
//            vesselItem->setFlags(vesselItem->flags()| Qt::ItemIsUserCheckable);
//            vesselItem->setCheckState(0, Qt::Unchecked);
//        }
//    }

//    QList<QString> files = this->patientHandling->getCenterLines();
//    for(int i=0 ; i<files.length(); i++){
//        QTreeWidgetItem *vesselItem = new QTreeWidgetItem(vesselsFolder);
//        vesselItem->setText(0, files.at(i));
//        vesselItem->setIcon(0, *this->fileUnloadedIcon);
//        vesselItem->setFlags(vesselItem->flags()| Qt::ItemIsUserCheckable);
//        vesselItem->setCheckState(0, Qt::Unchecked);
//    }
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::displayCenterLine
//!
void SurgeryPlanWindow::displayCenterLine(){

    int pointCount = this->patientHandling->getCenterLinePointsCount();

    if(pointCount > 0){
        poly->GetPointIds()->SetNumberOfIds(pointCount);

        for(int i = 0; i < pointCount; i++){
            poly->GetPointIds()->SetId(i,i);
        }

        this->grid->SetPoints(this->patientHandling->getCenterLinePoints());
        this->grid->InsertNextCell(poly->GetCellType(),poly->GetPointIds());
        this->mapperreferencePath->SetInputData(grid);
        this->actorreferencePath->SetMapper(mapperreferencePath);
        this->actorreferencePath->GetProperty()->SetColor(1,0,0);
        this->actorreferencePath->GetProperty()->SetOpacity(0.3);
        this->actorreferencePath->GetProperty()->SetPointSize(0.89);
        this->mainRenderer->AddActor(actorreferencePath);
    }
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::updatePatientMRAImage
//!
void SurgeryPlanWindow::displayPatientMRAImage(){
    display(this->patientHandling->getMraImageToBeDisplayed());
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::display
//! \param imgToBeDisplayed
//!
void SurgeryPlanWindow::display(vtkImageData *imgToBeDisplayed){
    this->mainRenderer->RemoveAllViewProps();
    //!---------------------------------------------------------------
    //! volume data visualization
    //!---------------------------------------------------------------

    this->volumeMapper->SetInputData(imgToBeDisplayed);
    //this->volumeMapper->SetBlendModeToComposite();
    this->volumeMapper->SetBlendModeToMaximumIntensity();
    this->volume->SetMapper(volumeMapper) ;
    this->volume->SetProperty(volumeProperty);
    //this->mainRenderer->AddActor(this->generateBoundBox(imgToBeDisplayed));
    this->mainRenderer->AddVolume(volume);
    this->renderWindow->AddRenderer(mainRenderer);

    this->patientMRAImage->SetRenderWindow(renderWindow);

    this->axes->SetInteractor(patientMRAImage->GetRenderWindow()->GetInteractor());
    this->axes->EnabledOn();
    this->axes->InteractiveOn();

    this->mainRenderer->ResetCamera();
    this->renderWindow->Render();
    this->patientMRAImage->update();

}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::setPatientHandling
//! \param patientHandling
//!
void SurgeryPlanWindow::setPatientHandling(Patient *patientHandling){
    this->patientHandling = patientHandling;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::displayWindow
//!
void SurgeryPlanWindow::displayWindow(){
    this->move(this->rect.x(), this->rect.y());
    this->resize(this->appWidth, this->appHeight);
    this->show();
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::displaySize
//!
void SurgeryPlanWindow::displaySize(){
    this->show();
    this->resize(appWidth, appHeight);
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::showContextMenu
//! \param pos
//!
void SurgeryPlanWindow::showContextMenu(const QPoint &pos){

    QTreeWidgetItem* vesselItem = centerlineTreeWidget->itemAt(pos);

    if(vesselItem == NULL){
        return;
    }

    vesselHandlingName = vesselItem->text(0);
    if(vesselHandlingName == "centerlines"){

    }
    else if(vesselHandlingName.contains("reference")||vesselHandlingName.contains("vessel")){
        QMenu* vesselsManipulationOptions = new QMenu(centerlineTreeWidget);
        QAction* loadAction = vesselsManipulationOptions->addAction("load");
        QAction* unloadAction =  vesselsManipulationOptions->addAction("unload");
        QAction* deleteAction =  vesselsManipulationOptions->addAction("delete");

        connect(loadAction,SIGNAL(triggered()),this,SLOT(loadVesselAction()));
        connect(unloadAction,SIGNAL(triggered()),this,SLOT(unloadVesselAction()));
        connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteVesselAction()));

        vesselsManipulationOptions->exec(QCursor::pos());
    }
    else{

    }

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::loadVesselAction
//!
void SurgeryPlanWindow::loadVesselAction(){
   this->patientHandling->loadVesselByPath("hehe",vesselHandlingName);
   this->displayVessel();
   this->displayCpr();
   this->doVesselReconstruct();

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::doVesselReconstruct
//!
void SurgeryPlanWindow::doVesselReconstruct(){
    QVector<CenterLinePoint*> vesselPoints = patientHandling->getVesselByName(vesselHandlingName);
    //qDebug()<<"doVesselReconstruct"<<vesselPoints;
    //qDebug()<<vesselHandlingName<<vesselPoints.size()<<vesselPoints.length();

    int cpt;
    for(cpt = 0; cpt<vesselPoints.size(); cpt++){
        vReconstructPoints->InsertPoint(cpt,vesselPoints.at(cpt)->get_abscissa(),vesselPoints.at(cpt)->get_ordinate(),vesselPoints.at(cpt)->get_isometric());
    }

    vReconstructLines->InsertNextCell(vesselPoints.size());

    for(cpt = 0; cpt<vesselPoints.size(); cpt++){
        vReconstructLines->InsertCellPoint(cpt);
    }

    vReconstructPolyData->SetPoints(vReconstructPoints);
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
    vReconstructActor->SetMapper(vReconstructMapper);
    vReconstructActor->GetProperty()->SetOpacity(0.4);
    vReconstructActor->GetProperty()->SetColor(1,0,0);

    this->singleVesselRenderer->AddActor(vReconstructActor);
    this->mainRenderer->AddActor(vReconstructActor);
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::displayVesselse
//!
void SurgeryPlanWindow::displayVessel(){
    centreLineCoordinates->clear();
    singleVesselPoints = patientHandling->getCenterlineByName(vesselHandlingName);
    singleVesselPoly->GetPointIds()->SetNumberOfIds(singleVesselPoints->GetNumberOfPoints());

    for(int i = 0;i<singleVesselPoints->GetNumberOfPoints();i++){
        double p0[3];
        singleVesselPoints->GetPoint(i, p0);
        centreLineCoordinates->addItem("(" + QString::number(p0[0]) + ", " + QString::number(p0[0]) + ", " + QString::number(p0[0]) + ")");
        centreLineCoordinates->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        singleVesselPoly->GetPointIds()->SetId(i,i);
    }

    singleVesselgrid->SetPoints(singleVesselPoints);
    singleVesselgrid->InsertNextCell(singleVesselPoly->GetCellType(),singleVesselPoly->GetPointIds());
    singleVesselMapper->SetInputData(singleVesselgrid);
    singleVesselactor->SetMapper(singleVesselMapper);
    singleVesselactor->GetProperty()->SetColor(0, 1, 1);
    singleVesselactor->GetProperty()->SetOpacity(1);
    singleVesselactor->GetProperty()->SetPointSize(0.2);

    singleVesselRenderer->AddActor(singleVesselactor);
    singleVesselRenderwindow->AddRenderer(singleVesselRenderer);
    centerLineVTKWidget->SetRenderWindow(singleVesselRenderwindow);
    centerLineVTKWidget->update();

    this->mainRenderer->AddActor(singleVesselactor);
    this->renderWindow->Render();
    //this->patientMRAImage->update();
}


//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::unloadVesselAction
//!
void SurgeryPlanWindow::unloadVesselAction(){
    //qDebug()<<"u failure load file";

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief deleteVesselAction
//!
void SurgeryPlanWindow::deleteVesselAction(){

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::stopSurgery
//!
void SurgeryPlanWindow::stopSurgery(){
    this->close();
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::updateCoords
//! \param obj
//!
void SurgeryPlanWindow::updateCoords(vtkObject* obj){
  // get interactor
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
  // get event position
  int event_pos[2];
  iren->GetEventPosition(event_pos);
  // update label
  QString str;
  str.sprintf("x=%d : y=%d", event_pos[0], event_pos[1]);
  //qDebug()<<str;
  //coord->setText(str);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::updatePatientMRAImageStatistics
//!
void SurgeryPlanWindow::updatePatientMRAImageStatistics(){

     QStringList statisticsList = patientHandling->getMriStatisticsList();

     grayscaleValueNumberLineEdit->setText(QString::number(patientHandling->getMraImageToBeDisplayed()->GetNumberOfPoints()));
     grayscaleValueMaximumValueLineEdit->setText(statisticsList[0]);
     grayscaleValueMinimumValueLineEdit->setText(statisticsList[1]);
     grayscaleValueMeanLineEdit->setText(statisticsList[2]);
     grayscaleValueMedianLineEdit->setText(statisticsList[3]);
     grayscaleValueStandardDeviationLineEdit->setText(statisticsList[4]);

}


//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::initialRendering
//!
void SurgeryPlanWindow::initialRendering(){

    int max = this->patientHandling->getMaximumGrayscaleValue();
    int min = this->patientHandling->getMinimumGrayscaleValue();
    int interval = max - min;

    //! doit etre calibrer selons les differentes valeurs maximaux
//    this->opacityTransferChoice->setChecked(true);
//    this->generateNewOpacityPoint(min ,               0.0);

//    this->generateNewOpacityPoint(900, 0.0);
//    this->generateNewOpacityPoint(1700, 1.8);
//    this->generateNewOpacityPoint(max,                1.8);

//    this->colorTransferChoice->setChecked(true);
//    this->generateInitColorPoints(min,                4);
//    this->generateInitColorPoints(max,                4);

    this->opacityTransferChoice->setChecked(true);

//    this->generateNewOpacityPoint(0 ,               0);
//    this->generateNewOpacityPoint(150 ,               0);
//    this->generateNewOpacityPoint(250 ,               1);
//    this->generateNewOpacityPoint(550 ,               1.5);
//    this->generateNewOpacityPoint(1000 ,               1.9);

    this->generateNewOpacityPoint(min ,               0.0);
    this->generateNewOpacityPoint(min + 0.3*interval, 0.0);
    this->generateNewOpacityPoint(min + 0.5*interval, 1);
    this->generateNewOpacityPoint(min + 0.7*interval, 1.8);
    this->generateNewOpacityPoint(max,                1.8);

    this->colorTransferChoice->setChecked(true);
//    this->generateInitColorPoints(0,               255, 255, 255 );
//    this->generateInitColorPoints(150,               255, 255, 255 );
//    this->generateInitColorPoints(250,               255, 215, 0 );
//    this->generateInitColorPoints(550,                255, 20, 147);
//    this->generateInitColorPoints(1000,                255,190,190);

    this->generateInitColorPoints(min,                4);
    this->generateInitColorPoints(min + 0.3*interval, 1);
    this->generateInitColorPoints(min + 0.5*interval, 2);
    this->generateInitColorPoints(min + 0.7*interval, 3);
    this->generateInitColorPoints(max,                4);

    this->gradientTransferChoice->setChecked(true);
    this->generateNewGradientPoint(min,                2.0);
    if(max  != 1){
        this->generateNewGradientPoint(min + 0.4*interval, 2.0);
        this->generateNewGradientPoint(min + 0.6*interval, 0.9);
        this->generateNewGradientPoint(min + 0.8*interval, 0.73);
    }
    this->generateNewGradientPoint(max,                0.1);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::transformationStateChanged
//!
void SurgeryPlanWindow::opacityTransformationStateChanged(bool state){
    transferOptionStates.opacityTransferOptionChoosen = state;
    if(transferOptionStates.opacityTransferOptionChoosen){
        this->updatePatientMRAImageTransformationCurves();
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::ColorTransformationStateChanged
//! \param state
//!
void SurgeryPlanWindow::colorTransformationStateChanged(bool state){
    transferOptionStates.colorTransferOptionChoosen = state;
    if(transferOptionStates.colorTransferOptionChoosen){
        this->updatePatientMRAImageTransformationCurves();
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::GradientTransformationStateChanged
//! \param state
//!
void SurgeryPlanWindow::gradientTransformationStateChanged(bool state){
    transferOptionStates.gradientTransferOptionChoosen = state;
    if(transferOptionStates.gradientTransferOptionChoosen){
        this->updatePatientMRAImageTransformationCurves();
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::process
//!
void SurgeryPlanWindow::process(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::updatePatientPersonelInformation
//!
void SurgeryPlanWindow::updatePatientPersonelInformation(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::updatePatientPersonelPhoto
//!
void SurgeryPlanWindow::updatePatientPersonelPhoto(){
    /*
    QString p = patientHandling->getPersonnelInfoPath() + patientHandling->getName() + ".png";
    photoWidget->setPixmap(QPixmap(p));
    photoWidget->setAutoFillBackground(true);
    photoWidget->setScaledContents(true);
    photoWidget->update();*/
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::updatePatientMRAImageHistogram
//!
void SurgeryPlanWindow::updatePatientMRAImageHistogram(){

    QVector<HistogramPoint*> frequencies = patientHandling->getMriHistogramfrequencies();

    int index = histogramPlottingBoard->addCurve("Histogram", "grayscale value", "", "cyan", 3);
    histogramPlottingBoard->doHistogramPlotting(index,frequencies);

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::opacityPointTracking
//!
void SurgeryPlanWindow::opacityPointTracking(){

    //!---------
    if(!this->transformationPlottingBoard_Clicked){
            //! -------------
            int index = this->patientHandling->findPointInTolerentArea(this->transformationPlottingBoard_AbscissaValue, this->transformationPlottingBoard_OrdinateValue, "opacity");
            if(index  == -1){
                transformationPlottingBoard->setCursor(Qt::ArrowCursor);
            }
            else if(index>=0){
                transformationPlottingBoard->setCursor(Qt::PointingHandCursor);
                currentGrayScaleValueLineEdit->setText(QString::number(this->patientHandling->getGrayScaleValueByIndex(index, "opacity")));
                currentTransformationValueLineEdit->setText(QString::number(this->patientHandling->getOpacityValueByIndex(index)));
                currentTransformationValueLabel->setText("opacity: ");
            }
    }
    else{
            //! ---------------
            if(lockingTransformationPointIndex >= 0){
                this->transformationPlottingBoard->setCursor(Qt::PointingHandCursor);
                this->patientHandling->updateOpacityPoint(lockingTransformationPointIndex, int(this->transformationPlottingBoard_AbscissaValue),  this->transformationPlottingBoard_OrdinateValue);
                this->transformationPlottingBoard->doTransformationPlotting(0,this->patientHandling->getOpacityTransferPoints());
                this->updatePatientMRAImage();
            }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::colorPointTracking
//!
void SurgeryPlanWindow::colorPointTracking(){

    //!---------
    if(!this->transformationPlottingBoard_Clicked){
        //! In grayscale -> color mode choosen, while the mouse's pointer moving through the plotting board, fetch the current color while the mouse's  pointer over a color point
        int index = this->patientHandling->findPointInTolerentArea(this->transformationPlottingBoard_AbscissaValue, this->transformationPlottingBoard_OrdinateValue, "color");
        if(index  == -1){
            transformationPlottingBoard->setCursor(Qt::ArrowCursor);
        }
        else if(index>=0){
            transformationPlottingBoard->setCursor(Qt::PointingHandCursor);
            currentGrayScaleValueLineEdit->setText(QString::number(this->patientHandling->getGrayScaleValueByIndex(index, "color")));
            currentTransformationValueLineEdit->setText("("+QString::number(this->patientHandling->getRedValueByIndex(index)) +
                                                        ", " + QString::number(this->patientHandling->getGreenValueByIndex(index)) +
                                                        ", " + QString::number(this->patientHandling->getBlueValueByIndex(index)) + ")");

            currentTransformationValueLineEdit->setStyleSheet("background-color: rgb("+
                                                              QString::number(this->patientHandling->getRedValueByIndex(index)*255) +
                                                              ", " + QString::number(this->patientHandling->getGreenValueByIndex(index)*255) +
                                                              ", " + QString::number(this->patientHandling->getBlueValueByIndex(index)*255) + ")"
                                                              ";color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px;");
        }
    }
    else{
        //qDebug()<<lockingTransformationPointIndex;
        if(lockingTransformationPointIndex >= 0){
            this->transformationPlottingBoard->setCursor(Qt::PointingHandCursor);
            this->patientHandling->updateColorPoint(lockingTransformationPointIndex, int(this->transformationPlottingBoard_AbscissaValue));
            this->transformationPlottingBoard->doColorTransformationPlotting(this->patientHandling->getColorTransferPoints());
            this->updatePatientMRAImage();
        }
    }

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::gradientPointTracking
//!
void SurgeryPlanWindow::gradientPointTracking(){

    //!---------
    if(!this->transformationPlottingBoard_Clicked){
        int index = this->patientHandling->findPointInTolerentArea(this->transformationPlottingBoard_AbscissaValue, this->transformationPlottingBoard_OrdinateValue, "gradient");
        if(index  == -1){
            transformationPlottingBoard->setCursor(Qt::ArrowCursor);
        }
        else if(index>=0){
            transformationPlottingBoard->setCursor(Qt::PointingHandCursor);
            currentGrayScaleValueLineEdit->setText(QString::number(this->patientHandling->getGrayScaleValueByIndex(index, "gradient")));
            currentTransformationValueLineEdit->setText(QString::number(this->patientHandling->getGradientValueByIndex(index)));
            currentTransformationValueLabel->setText("gradient: ");
        }
    }
    else{

        if(lockingTransformationPointIndex >= 0){
            this->transformationPlottingBoard->setCursor(Qt::PointingHandCursor);
            this->patientHandling->updateGradientPoint(lockingTransformationPointIndex, int(this->transformationPlottingBoard_AbscissaValue), this->transformationPlottingBoard_OrdinateValue);
            //this->transformationPlottingBoard->clearGraphs();
            this->transformationPlottingBoard->doTransformationPlotting(0,this->patientHandling->getGradientTransferPoints());
            this->updatePatientMRAImage();
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::tranformPointTracking
//!
void SurgeryPlanWindow::transformPointTracking(QMouseEvent*event){

    this->transformationPlottingBoard_AbscissaValue = transformationPlottingBoard->xAxis->pixelToCoord(event->pos().x());
    this->transformationPlottingBoard_OrdinateValue = transformationPlottingBoard->yAxis->pixelToCoord(event->pos().y());
    if(transferOptionStates.opacityTransferOptionChoosen){
       this->opacityPointTracking();
    }
    else if(transferOptionStates.colorTransferOptionChoosen){
       this->colorPointTracking();
    }
    else{
       this->gradientPointTracking();
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::lockTargetPoint
//! \param event
//!
void SurgeryPlanWindow::lockTargetPoint(QMouseEvent* event){


    this->transformationPlottingBoard_AbscissaValue = transformationPlottingBoard->xAxis->pixelToCoord(event->pos().x());
    this->transformationPlottingBoard_OrdinateValue = transformationPlottingBoard->yAxis->pixelToCoord(event->pos().y());
    if(event->button() == Qt::LeftButton) {
        this->transformationPlottingBoard_Clicked = true;
        if(transferOptionStates.opacityTransferOptionChoosen){
            lockingTransformationPointIndex = this->patientHandling->findPointInTolerentArea(this->transformationPlottingBoard_AbscissaValue, this->transformationPlottingBoard_OrdinateValue,  "opacity");
        }
        else if(transferOptionStates.colorTransferOptionChoosen){
            lockingTransformationPointIndex = this->patientHandling->findPointInTolerentArea(this->transformationPlottingBoard_AbscissaValue, this->transformationPlottingBoard_OrdinateValue,  "color");
        }
        else{
            lockingTransformationPointIndex = this->patientHandling->findPointInTolerentArea(this->transformationPlottingBoard_AbscissaValue,  this->transformationPlottingBoard_OrdinateValue, "gradient");
        }
    }
    else{

    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::positioningTransformPoint
//! \param event
//!
void SurgeryPlanWindow::positioningTransformPoint(QMouseEvent* event){
    if(event->button() == Qt::LeftButton) {
        this->transformationPlottingBoard_AbscissaValue = transformationPlottingBoard->xAxis->pixelToCoord(event->pos().x());
        this->transformationPlottingBoard_OrdinateValue = transformationPlottingBoard->yAxis->pixelToCoord(event->pos().y());
        this->transformationPlottingBoard_Clicked = false;
        this->lockingTransformationPointIndex = -1;
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::generateNewTransformationPoint
//! \param event
//!
void SurgeryPlanWindow::generateNewTransformationPoint(QMouseEvent* event){
    this->transformationPlottingBoard_AbscissaValue = transformationPlottingBoard->xAxis->pixelToCoord(event->pos().x());
    this->transformationPlottingBoard_OrdinateValue = transformationPlottingBoard->yAxis->pixelToCoord(event->pos().y());

    if(event->button() == Qt::LeftButton) {
        if(transferOptionStates.opacityTransferOptionChoosen){
            this->generateNewOpacityPoint(this->transformationPlottingBoard_AbscissaValue,  this->transformationPlottingBoard_OrdinateValue);
        }
        else if(transferOptionStates.colorTransferOptionChoosen){
            this->generateNewColorPoints(this->transformationPlottingBoard_AbscissaValue,  this->transformationPlottingBoard_OrdinateValue);
        }
        else{
            this->generateNewGradientPoint(this->transformationPlottingBoard_AbscissaValue,  this->transformationPlottingBoard_OrdinateValue);
        }
    }
    else{

    }

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::generateNewOpacityPoint
//! \param abscissa
//! \param ordinate
//!
void SurgeryPlanWindow::generateNewOpacityPoint(double abscissa, double ordinate){

    TransferPoint*opacityPoint = new TransferPoint();
    opacityPoint->setAbscissaValue(abscissa);
    opacityPoint->setOrdinateValue(ordinate);
    this->patientHandling->setOpacityTransferPoint(opacityPoint);
    if(transferOptionStates.opacityTransferOptionChoosen){
        this->transformationPlottingBoard->doTransformationPlotting(0,this->patientHandling->getOpacityTransferPoints());
    }
    this->updatePatientMRAImage();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::generateNewColorPoints
//! \param abscissa
//! \param ordinate
//!
void SurgeryPlanWindow::generateNewColorPoints(double abscissa, double ordinate){
    ColorPoint * colorPoint =  new ColorPoint();

    //TODO: display a widget for color chosen
    colorChooseWindow->exec();

    if(colorChooseWindow->colorChosen()){
        colorPoint->setAbscissaValue(abscissa);

        colorPoint->setRedValue(1.0*colorChooseWindow->getRedValue()/255);
        colorPoint->setGreenValue(1.0*colorChooseWindow->getGreenValue()/255);
        colorPoint->setBlueValue(1.0*colorChooseWindow->getBlueValue()/255);
        this->patientHandling->setColorTransferPoint(colorPoint);
        if(transferOptionStates.colorTransferOptionChoosen){
            this->transformationPlottingBoard->doColorTransformationPlotting(this->patientHandling->getColorTransferPoints());
        }
        this->updatePatientMRAImage();
    }

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::generateNewGradientPoint
//! \param abscissa
//! \param ordinate
//!
void SurgeryPlanWindow::generateNewGradientPoint(double abscissa, double ordinate){
    TransferPoint*opacityPoint = new TransferPoint();
    opacityPoint->setAbscissaValue(abscissa);
    opacityPoint->setOrdinateValue(ordinate);
    this->patientHandling->setGradientTransferPoint(opacityPoint);
     if(transferOptionStates.gradientTransferOptionChoosen){
        this->transformationPlottingBoard->doTransformationPlotting(0,this->patientHandling->getGradientTransferPoints());
     }
    this->updatePatientMRAImage();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief generateNewColorPoint
//! \param abscissa
//! \param ordinate
//! \param color
//!
void SurgeryPlanWindow::generateInitColorPoints(double abscissa, int colorCount){

    QColor *color =  new QColor(255, colorCount*51, 0);
    ColorPoint *colorPoint = new ColorPoint();
    colorPoint->setAbscissaValue(abscissa);
    colorPoint->setBlueValue(1.0*color->blue()/255);
    colorPoint->setRedValue(1.0*color->red()/255);
    colorPoint->setGreenValue(1.0*color->green()/255);

    this->patientHandling->setColorTransferPoint(colorPoint);
    if(transferOptionStates.colorTransferOptionChoosen){
        this->transformationPlottingBoard->doColorTransformationPlotting(this->patientHandling->getColorTransferPoints());
    }
   this->updatePatientMRAImage();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::generateInitColorPoints
//! \param abscissa
//! \param red
//! \param green
//! \param blue
//!
void SurgeryPlanWindow::generateInitColorPoints(double abscissa, int red, int green, int blue){
    QColor *color =  new QColor(red, green, blue);
    ColorPoint *colorPoint = new ColorPoint();
    colorPoint->setAbscissaValue(abscissa);
    colorPoint->setBlueValue(1.0*color->blue()/255);
    colorPoint->setRedValue(1.0*color->red()/255);
    colorPoint->setGreenValue(1.0*color->green()/255);

    this->patientHandling->setColorTransferPoint(colorPoint);
    if(transferOptionStates.colorTransferOptionChoosen){
        this->transformationPlottingBoard->doColorTransformationPlotting(this->patientHandling->getColorTransferPoints());
    }
   this->updatePatientMRAImage();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::updatePatientMRAImage
//!
void SurgeryPlanWindow::updatePatientMRAImage(){
    if(transferOptionStates.opacityTransferOptionChoosen){
        this->opacityTransferFunction->RemoveAllPoints();
        QVector<TransferPoint*> points = this->patientHandling->getOpacityTransferPoints();
        for(unsigned char cpt = 0; cpt < points.size(); cpt++){
            this->opacityTransferFunction->AddPoint(int(points[cpt]->getAbscissaValue()), points[cpt]->getOrdinateValue());
        }
        this->volumeProperty->SetScalarOpacity(opacityTransferFunction);
    }
    else if(transferOptionStates.colorTransferOptionChoosen){
        this->colorTransferFunction->RemoveAllPoints();
        QVector<ColorPoint*> points = this->patientHandling->getColorTransferPoints();
        for(unsigned char cpt = 0; cpt < points.size(); cpt++){
            this->colorTransferFunction->AddRGBPoint(int(points[cpt]->getAbscissaValue()), points[cpt]->getRedValue(), points[cpt]->getGreenValue(), points[cpt]->getBlueValue());
        }
        volumeProperty->SetColor(colorTransferFunction);
    }
    else{
        this->gradientTransferFunction->RemoveAllPoints();
        QVector<TransferPoint*> points = this->patientHandling->getGradientTransferPoints();
        for(unsigned char cpt = 0; cpt < points.size(); cpt++){
            this->gradientTransferFunction->AddPoint(int(points[cpt]->getAbscissaValue()), points[cpt]->getOrdinateValue());
        }
        this->volumeProperty->SetGradientOpacity(gradientTransferFunction);
    }
    //this->volume->SetProperty(volumeProperty); modified 2015/11/25
    this->patientMRAImage->update();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::updatePatientMRAImageTransformationCurves
//!
void SurgeryPlanWindow::updatePatientMRAImageTransformationCurves(){
    if(transformationPlottingBoard->getCurveNumber()>0){
        transformationPlottingBoard->clearGraphs();
    }

    transformationPlottingBoard->setAbscissaRange(this->patientHandling->getMriStatisticsList()[1].toInt(0,10),
                                                  this->patientHandling->getMriStatisticsList()[0].toInt(0,10)*1.1 );

    if(transferOptionStates.opacityTransferOptionChoosen){
        currentTransformationValueLabel->setText("Opacity:  ");
        currentTransformationValueLineEdit->setStyleSheet("QLineEdit {background-color:transparent; color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange;}");
        transformationPlottingBoard->setOrdinateRange(-0.5, 2.5);
        transformationPlottingBoard->legendSetting(false);
        transferCurveIndex.opacityTransferCurveIndex = transformationPlottingBoard->addCurve("grayValue->opacity", "grayscale value", "", "gainsboro", 7);
        this->transformationPlottingBoard->doTransformationPlotting(0,this->patientHandling->getOpacityTransferPoints());
    }
    else if(transferOptionStates.colorTransferOptionChoosen){
        currentTransformationValueLabel->setText("Color:    ");
        transformationPlottingBoard->setOrdinateRange(-0.5, 2.5);
         transformationPlottingBoard->legendSetting(true);
        transferCurveIndex.colorTransferCurveIndex = transformationPlottingBoard->addCurve("grayValue->color", "grayscale value", "", "gainsboro", 7);
        this->transformationPlottingBoard->doColorTransformationPlotting(this->patientHandling->getColorTransferPoints());
    }
    else{
        currentTransformationValueLabel->setText("Gradient: ");
        currentTransformationValueLineEdit->setStyleSheet("QLineEdit {background-color:transparent; color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange;}");
        transformationPlottingBoard->legendSetting(false);
        transferCurveIndex.gradientTransferCurveIndex = transformationPlottingBoard->addCurve("grayValue->gradient", "grayscale value", "", "gainsboro", 7);
        this->transformationPlottingBoard->doTransformationPlotting(0,this->patientHandling->getGradientTransferPoints());
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief Patient::removeCurveBy
//! \param index
//!
void SurgeryPlanWindow::removeCurveBy(int index){
    transformationPlottingBoard->removeGraph(index);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::constructControlBar
//!
void SurgeryPlanWindow::constructControlBar(){
    controlBar = new QFrame();
    controlBar->setFixedHeight(int(appHeight*0.025));
    //controlBar->setStyleSheet("background-color: orange");

    controlBarLayout = new QHBoxLayout(controlBar);

    imageConfigurationButton = new CPushButton();
    imageConfigurationButton->setIcon(QIcon(":/images/image_configuration.png"));
    imageConfigurationButton->setFixedSize(QSize(int(appHeight*0.025), int(appHeight*0.025)));
    imageConfigurationButton->setIconSize(QSize(int(appHeight*0.025), int(appHeight*0.025)));
    imageConfigurationButton->setFlat(true);

    imageUpdateButton = new CPushButton();
    imageUpdateButton->setIcon(QIcon(":/images/image_update.png"));
    imageUpdateButton->setFixedSize(QSize(int(appHeight*0.025), int(appHeight*0.025)));
    imageUpdateButton->setIconSize(QSize(int(appHeight*0.025), int(appHeight*0.025)));
    imageUpdateButton->setFlat(true);

    dislayImageAnalyseAreaButton = new CPushButton();
    dislayImageAnalyseAreaButton->setIcon(QIcon(":/images/pushup.png"));
    dislayImageAnalyseAreaButton->setFixedSize(QSize(int(appHeight*0.025),int(appHeight*0.025)));
    dislayImageAnalyseAreaButton->setIconSize(QSize(int(appHeight*0.025),int(appHeight*0.025)));
    dislayImageAnalyseAreaButton->setFlat(true);

    beginTherapyButton = new CPushButton();
    beginTherapyButton->setIcon(QIcon(":/images/begin_therapy.png"));
    beginTherapyButton->setFixedSize(QSize(int(appHeight*0.025), int(appHeight*0.025)));
    beginTherapyButton->setIconSize(QSize(int(appHeight*0.025), int(appHeight*0.025)));
    beginTherapyButton->setFlat(true);
    spacer_controlbar = new QSpacerItem(int(appHeight*0.025), 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    controlBarLayout->addWidget(imageConfigurationButton);
    controlBarLayout->addWidget(imageUpdateButton);
    controlBarLayout->addWidget(dislayImageAnalyseAreaButton);
    controlBarLayout->addItem(spacer_controlbar);
    controlBarLayout->addWidget(beginTherapyButton);
    controlBarLayout->setSpacing(0);
    controlBarLayout->setMargin(0);
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! Initialisation des composant necessaire pour l'affichage des images tridimensionelle
//!
//! \brief SurgeryPlanWindow::initialisation
//!
void SurgeryPlanWindow::initialisation(){

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);// | Qt::WindowStaysOnTopHint);
    this->setWindowOpacity(1);
    this->setMouseTracking(true);
    this->setAutoFillBackground(true);

    this->defaultFolderIcon = new QIcon(":/images/folder.png");
    this->fileUnloadedIcon = new QIcon(":/images/fileunloaded.png");
    this->fileLoadedIcon = new QIcon(":/images/fileloaded.png");
    this->defaultTitleIcon = new QIcon(":/images/title.png");
    this->font = new  QFont("Helvetica", 8, QFont::AnyStyle, true);

    //fixedPointVolumeRayCastMapper = vtkFixedPointVolumeRayCastMapper::New();
    //! for volume data visualization
    volumeMapper = vtkFixedPointVolumeRayCastMapper::New();
    volume = vtkVolume::New();
    renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    mainRenderer = vtkSmartPointer<vtkRenderer>::New();

    opacityTransferFunction=vtkPiecewiseFunction::New();
    gradientTransferFunction = vtkPiecewiseFunction::New();
    colorTransferFunction = vtkColorTransferFunction::New();

    volumeProperty=vtkVolumeProperty::New();
    volumeProperty->ShadeOff();
    volumeProperty->SetInterpolationTypeToLinear();

    //! for centerline visualization
    actorreferencePath = vtkActor::New();
    mapperreferencePath = vtkDataSetMapper::New();
    grid = vtkUnstructuredGrid::New();
    poly = vtkPolyVertex::New();

    singleVesselPoly = vtkPolyVertex::New();
    singleVesselgrid = vtkUnstructuredGrid::New();
    singleVesselMapper =  vtkDataSetMapper::New();
    singleVesselactor = vtkActor::New();
    singleVesselRenderer = vtkRenderer::New();
    singleVesselRenderwindow =vtkRenderWindow::New();

    //! cpr
    cprmapper = vtkPolyDataMapper::New();
    cpractor = vtkActor::New();
    cprRenderer = vtkRenderer::New();
    cprRenderwindow = vtkRenderWindow::New();

    imageOptionStates.originalOptionState = false;
    imageOptionStates.transparentBrainOptionState = false;
    imageOptionStates.greyMatterOptionState = false;
    imageOptionStates.whiteMatterOptionState = false;
    imageOptionStates.vesselOptionState = false;
    imageOptionStates.interventionalRouteOptionState = false;

    transformationPlottingBoard_Clicked = false;
    lockingTransformationPointIndex = -1;

    this->transformationPlottingBoard_AbscissaValue = 0.0;
    this->transformationPlottingBoard_OrdinateValue = 0.0;

    this->patientWidgetConfigurationBoard = new PatientWidgetConfigurationBoard();
    this->patientWidgetConfigurationBoard->setPatientHandling(this->patientHandling);
    this->colorChooseWindow = new ColorChooseWindow();

    this->timerToWaittingReconstructedResult = new QTimer(this);

    //! vesselReconstruct
    vReconstructPoints = vtkPoints::New();
    vReconstructLines = vtkCellArray::New();
    vReconstructPolyData = vtkPolyData::New();
    vReconstructRadius = vtkDoubleArray::New();
    vReconstructTube = vtkTubeFilter::New();
    vReconstructMapper = vtkPolyDataMapper::New();
    vReconstructActor = vtkActor::New();


    displayImageAnalyseAreaButtonClicked = false;

    this->grayMatterActor = NULL;
    this->whiteMatterActor = NULL;
    //this->vesselActor = NULL;
    this->vesselLeftActor = NULL;
    this->vesselRightActor = NULL;
    this->vesselFrontActor = NULL;
    this->vesselBackActor = NULL;

    QString humaBodyDataPath = this->systemDispatcher->getImageCenterPath()+"human_body.obj";
    vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
    reader->SetFileName(humaBodyDataPath.toLocal8Bit().data());
    reader->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    axes = vtkOrientationMarkerWidget::New();
    axes->SetOrientationMarker(actor);

    realTimeTrackingNDITimer = new QTimer();
    previousNDIPosActor = NULL;
    this->ndiInfoCount = 0;

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::configuration
//!
void SurgeryPlanWindow::configuration(){
    axes->SetDefaultRenderer(this->mainRenderer);

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! Construct the widget which contains all the information about the patient in order to prepare the therapy
//!
//! \brief SurgeryPlanWindow::constructPatientInformationWidget
//!
void SurgeryPlanWindow::constructTridimensionalImageVisualisingArea(){

    this->elapsedTimeLabel = new QLCDNumber();
    this->elapsedTimeLabel->setStyleSheet("background-color: transparent; color: Gainsboro");
    this->elapsedTimeLabel->setDigitCount(8);
    this->elapsedTimeLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));

    this->surgeryPlanWindowLabel = new QLabel();
    this->surgeryPlanWindowLabel->setText("--Surgery Plan--");
    this->surgeryPlanWindowLabel->setStyleSheet("QLabel{background-color: transparent; color: Orange}");
    this->surgeryPlanWindowLabel->setFont(QFont("Segoe UI", 10, QFont::DemiBold, true));
    this->surgeryPlanWindowLabel->setAlignment(Qt::AlignLeft);

    patientImageInformationWidget = new QWidget();
    patientImageInformationWidget->setFixedHeight(int(appHeight*0.03));
    patientImageInformationWidgetLayout = new QHBoxLayout(patientImageInformationWidget);
    patientImageInformationWidgetLayout->addWidget(surgeryPlanWindowLabel);
    //! TODO to be customerized
    //patientImageInformationWidgetLayout->addWidget(elapsedTimeLabel);
    patientImageInformationWidgetLayout->setSpacing(0);
    patientImageInformationWidgetLayout->setMargin(0);

    imageStatisticsWidget = new QWidget();
    imageStatisticsWidget->setFixedHeight(appHeight*0.15);
    imageStatisticsWidgetLayout = new QGridLayout(imageStatisticsWidget);

    grayscaleValueNumberLabel = new QLabel("Total Number: ");
    grayscaleValueMeanLabel = new QLabel("Mean: ");
    grayscaleValueMedianLabel = new QLabel("Median: ");
    grayscaleValueMaximumValueLabel = new QLabel("Maximum Value: ");
    grayscaleValueMinimumValueLabel = new QLabel("Minimum Value: ");
    grayscaleValueStandardDeviationLabel = new QLabel("Standard Deviation: ");

    grayscaleValueNumberLineEdit = new QLineEdit();
    grayscaleValueMeanLineEdit = new QLineEdit();
    grayscaleValueMedianLineEdit = new QLineEdit();
    grayscaleValueMaximumValueLineEdit = new QLineEdit();
    grayscaleValueMinimumValueLineEdit = new QLineEdit();
    grayscaleValueStandardDeviationLineEdit = new QLineEdit();

    grayscaleValueNumberLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMeanLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMedianLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMaximumValueLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMinimumValueLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueStandardDeviationLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));

    grayscaleValueNumberLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMeanLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMedianLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMaximumValueLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMinimumValueLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueStandardDeviationLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));

    grayscaleValueNumberLineEdit->setReadOnly(true);
    grayscaleValueMeanLineEdit->setReadOnly(true);
    grayscaleValueMedianLineEdit->setReadOnly(true);
    grayscaleValueMaximumValueLineEdit->setReadOnly(true);
    grayscaleValueMinimumValueLineEdit->setReadOnly(true);
    grayscaleValueStandardDeviationLineEdit->setReadOnly(true);

    grayscaleValueNumberLabel->setStyleSheet("color: orange");
    grayscaleValueMeanLabel->setStyleSheet("color: orange");
    grayscaleValueMedianLabel->setStyleSheet("color: orange");
    grayscaleValueMaximumValueLabel->setStyleSheet("color: orange");
    grayscaleValueMinimumValueLabel->setStyleSheet("color: orange");
    grayscaleValueStandardDeviationLabel->setStyleSheet("color: orange");

    grayscaleValueNumberLineEdit->setStyleSheet("QLineEdit {background-color:transparent; color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange;}");
    grayscaleValueMeanLineEdit->setStyleSheet("QLineEdit {background-color:transparent; color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange; }");
    grayscaleValueMedianLineEdit->setStyleSheet("QLineEdit {background-color:transparent; color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange; }");
    grayscaleValueMaximumValueLineEdit->setStyleSheet("QLineEdit {background-color:transparent; color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange; }");
    grayscaleValueMinimumValueLineEdit->setStyleSheet("QLineEdit {background-color:transparent; color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange; }");
    grayscaleValueStandardDeviationLineEdit->setStyleSheet("QLineEdit {background-color:transparent; color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange; }");

    imageStatisticsWidgetLayout->addWidget(grayscaleValueNumberLabel, 0, 0);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueNumberLineEdit, 0, 1);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueMaximumValueLabel, 1, 0);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueMaximumValueLineEdit, 1, 1);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueMinimumValueLabel, 2, 0);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueMinimumValueLineEdit, 2, 1);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueMeanLabel, 3, 0);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueMeanLineEdit, 3, 1);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueMedianLabel, 4, 0);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueMedianLineEdit, 4, 1);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueStandardDeviationLabel, 5, 0);
    imageStatisticsWidgetLayout->addWidget(grayscaleValueStandardDeviationLineEdit, 5, 1);
    imageStatisticsWidgetLayout->setSpacing(0);

    histogramPlottingBoard = new PlottingBoard();
    histogramPlottingBoard->setFixedHeight(appHeight*0.2);
    histogramPlottingBoard->legendSetting(true);

    histogramGroupBox = new QGroupBox();
    histogramGroupBox->setStyleSheet("QGroupBox{background-color:transparent; color: orange; border: 1px solid orange; border-radius: 0px;margin-top: 1ex;} ");
    histogramGroupBox->setTitle("Image Statistics");
    histogramGroupBox->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    histogramGroupBoxLayout = new QVBoxLayout(histogramGroupBox);
    histogramGroupBoxLayout->addWidget(imageStatisticsWidget);
    histogramGroupBoxLayout->addWidget(histogramPlottingBoard);

    histogramGroupBoxLayout->setSpacing(0);
    histogramGroupBoxLayout->setContentsMargins(0,15,0,0);

    //!-------------------------------------
    volumeRenderingGroupBox = new QGroupBox();
    volumeRenderingGroupBox->setStyleSheet("QGroupBox{background-color:transparent; color: orange; border: 1px solid orange; border-radius: 0px;margin-top: 1ex;} ");
    volumeRenderingGroupBox->setTitle("Volume Rendering");
    volumeRenderingGroupBox->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    volumeRenderingGroupBoxLayout = new QVBoxLayout(volumeRenderingGroupBox);

    transferChoicesWidget = new QWidget();
    transferChoicesLayout = new QHBoxLayout(transferChoicesWidget);

    transferChoiceLabel = new QLabel("transfer grayscale value to: ");
    opacityTransferChoice = new QRadioButton("opacity");
    colorTransferChoice = new QRadioButton("color");
    gradientTransferChoice = new QRadioButton("gradient");
    transferChoicesWidgetSpacer = new QSpacerItem(0,10, QSizePolicy::Expanding, QSizePolicy::Expanding);

    opacityTransferChoice->setFixedWidth(70);
    colorTransferChoice->setFixedWidth(60);
    gradientTransferChoice->setFixedWidth(70);

    transferChoiceLabel->setStyleSheet("color: orange");
    opacityTransferChoice->setStyleSheet("color: orange");
    colorTransferChoice->setStyleSheet("color: orange");
    gradientTransferChoice->setStyleSheet("color: orange");

    transferChoiceLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    opacityTransferChoice->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    colorTransferChoice->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    gradientTransferChoice->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));

    transferChoicesLayout->addWidget(transferChoiceLabel);
    transferChoicesLayout->addWidget(opacityTransferChoice);
    transferChoicesLayout->addWidget(colorTransferChoice);
    transferChoicesLayout->addWidget(gradientTransferChoice);
    transferChoicesLayout->addItem(transferChoicesWidgetSpacer);
    transferChoicesLayout->setSpacing(0);

    transformationPlottingBoard = new PlottingBoard();
    transformationPlottingBoard->setWorkSpaceColor(55,85,95);

    transformationIndicationBar = new QWidget();
    transformationIndicationBarLayout = new QHBoxLayout(transformationIndicationBar);
    currentGrayScaleValueLabel = new QLabel("grascale value: ");
    currentTransformationValueLabel = new QLabel("opacity: ");
    transformationButton = new CPushButton(this);

    transformationButton->setIcon(QIcon(":/images/transformation.png"));
    transformationButton->setFixedSize(QSize(60, 30));
    transformationButton->setIconSize(QSize(53, 23));
    transformationButton->setFlat(true);

    currentGrayScaleValueLineEdit = new QLineEdit();
    currentTransformationValueLineEdit = new QLineEdit();
    currentGrayScaleValueLabel->setStyleSheet("color: orange");
    currentTransformationValueLabel->setStyleSheet("color: orange");
    currentGrayScaleValueLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    currentTransformationValueLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));

    grayscaleValueNumberLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMeanLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMedianLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMaximumValueLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueMinimumValueLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    grayscaleValueStandardDeviationLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));

    currentGrayScaleValueLineEdit->setReadOnly(true);
    currentTransformationValueLineEdit->setReadOnly(true);
    currentGrayScaleValueLineEdit->setStyleSheet("QLineEdit {background-color:transparent; color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange;}");
    currentTransformationValueLineEdit->setStyleSheet("QLineEdit {background-color:transparent; color:orange; border: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange;}");

    transformationIndicationBarLayout->addWidget(currentGrayScaleValueLabel);
    transformationIndicationBarLayout->addWidget(currentGrayScaleValueLineEdit);
    transformationIndicationBarLayout->addWidget(transformationButton);
    transformationIndicationBarLayout->addWidget(currentTransformationValueLabel);
    transformationIndicationBarLayout->addWidget(currentTransformationValueLineEdit);
    transformationIndicationBarLayout->setSpacing(0);

    transformationPlottingBoard->setFixedHeight(appHeight*0.2);
    transformationPlottingBoard->legendSetting(false);

    volumeRenderingGroupBoxLayout->addWidget(transferChoicesWidget);
    volumeRenderingGroupBoxLayout->addWidget(transformationPlottingBoard);
    volumeRenderingGroupBoxLayout->addWidget(transformationIndicationBar);
    volumeRenderingGroupBoxLayout->setSpacing(0);

    patientMRAImageOptionWidgetSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    this->patientMRAImageOptionWidget =  new QWidget();
    this->patientMRAImageOptionWidget->setFixedWidth(int(appWidth*0.3));
    this->patientMRAImageOptionWidgetLayout = new QVBoxLayout(patientMRAImageOptionWidget);
    this->patientMRAImageOptionWidgetLayout->addWidget(patientImageInformationWidget);
    this->patientMRAImageOptionWidgetLayout->addWidget(histogramGroupBox);
    this->patientMRAImageOptionWidgetLayout->addWidget(volumeRenderingGroupBox);
    this->patientMRAImageOptionWidgetLayout->addItem(patientMRAImageOptionWidgetSpacer);
    this->patientMRAImageOptionWidgetLayout->setSpacing(0);
    this->patientMRAImageOptionWidgetLayout->setMargin(0);

    //!--------------------------------------------------------------------------------------------------------
    //! six item where you could select different image of patient's MRA SurgeryPlanWindowmage
    //!--------------------------------------------------------------------------------------------------------
    this->originalOption = new CPushButton();
    this->originalOption->setText("Original Image");
    this->originalOption->setFixedSize(QSize(150, int(appHeight*0.03)));
    this->originalOption->setFixedWidth(150);
    this->originalOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    this->originalOption->setFlat(true);
    this->originalOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));

    this->transparentBrainOption = new CPushButton();
    this->transparentBrainOption->setText("Transparent Brain");
    this->transparentBrainOption->setFixedSize(QSize(150, int(appHeight*0.03)));
    this->transparentBrainOption->setFixedWidth(150);
    this->transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    this->transparentBrainOption->setFlat(true);
    this->transparentBrainOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));

    this->greyMatterOption = new CPushButton();
    this->greyMatterOption->setText("Gray Matter");
    this->greyMatterOption->setFixedSize(QSize(150, int(appHeight*0.03)));
    this->greyMatterOption->setFixedWidth(150);
    this->greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    this->greyMatterOption->setFlat(true);
    this->greyMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

    this->whiteMatterOption = new CPushButton();
    this->whiteMatterOption->setText("White Matter");
    this->whiteMatterOption->setFixedSize(QSize(150, int(appHeight*0.03)));
    this->whiteMatterOption->setFixedWidth(150);
    this->whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    this->whiteMatterOption->setFlat(true);
    this->whiteMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

    this->vesselOption = new CPushButton();
    this->vesselOption->setText("Vessel Tree");
    this->vesselOption->setFixedSize(QSize(150, int(appHeight*0.03)));
    this->vesselOption->setFixedWidth(150);
    this->vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    this->vesselOption->setFlat(true);
    this->vesselOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

    this->interventionalRouteOption = new CPushButton();
    this->interventionalRouteOption->setText("Interventional Route");
    this->interventionalRouteOption->setFixedSize(QSize(150, int(appHeight*0.03)));
    this->interventionalRouteOption->setFixedWidth(appWidth*0.16);
    this->interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    this->interventionalRouteOption->setFlat(true);
    this->interventionalRouteOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

    this->patientMRAImageOptionLabel = new QLabel();

    this->quitSurgeryPlanButton = new QPushButton("X");
    this->quitSurgeryPlanButton->setFixedSize(int(appHeight*0.03), int(appHeight*0.03));
    this->quitSurgeryPlanButton->setStyleSheet("background-color:transparent");

    //!--------------------------------------------------------------------------------------------------------
    //! patient MRA image  widget and its horizonal layout (the widget which is used to display MRA image)
    //!--------------------------------------------------------------------------------------------------------
    this->patientMRAImageOption = new QWidget();
    this->patientMRAImageOption->setFixedHeight(int(appHeight*0.03));
    this->patientMRAImageOptionLayout = new QHBoxLayout(this->patientMRAImageOption);
    this->patientMRAImageOptionLayout->addWidget(this->originalOption);
    this->patientMRAImageOptionLayout->addWidget(this->transparentBrainOption);
    this->patientMRAImageOptionLayout->addWidget(this->greyMatterOption);
    this->patientMRAImageOptionLayout->addWidget(this->whiteMatterOption);
    this->patientMRAImageOptionLayout->addWidget(this->vesselOption);
    this->patientMRAImageOptionLayout->addWidget(this->interventionalRouteOption);
    this->patientMRAImageOptionLayout->addWidget(this->patientMRAImageOptionLabel);
    this->patientMRAImageOptionLayout->addWidget(this->quitSurgeryPlanButton);
    this->patientMRAImageOptionLayout->setSpacing(1);
    this->patientMRAImageOptionLayout->setMargin(0);

    //!--------------------------------------------------------------------------------------------------------
    //! patient MRA image  widget and its horizonal layout (the widget which is used to display MRA image)
    //!--------------------------------------------------------------------------------------------------------
    this->patientMRAImageWidget = new QWidget();
    this->patientMRAImageWidgetLayout = new QHBoxLayout(this->patientMRAImageWidget);
    this->patientMRAImage = new QVTKWidget();

    //patientMRAImageManipulation = new QWidget();
    this->patientMRAImageWidgetLayout->addWidget(this->patientMRAImage);
    this->patientMRAImageWidgetLayout->setSpacing(0);
    this->patientMRAImageWidgetLayout->setMargin(0);

    //!--------------------------------------------------------------------------------------------------------
    //! patient MRA image configuration widget and its vertical layout
    //!--------------------------------------------------------------------------------------------------------
    this->patientMRAImageConfigurationWidget =  new QWidget();
    this->patientMRAImageConfigurationWidgetLayout = new QVBoxLayout(this->patientMRAImageConfigurationWidget);
    this->patientMRAImageConfigurationWidgetLayout->addWidget(this->patientMRAImageOption);
    this->patientMRAImageConfigurationWidgetLayout->addWidget(this->patientMRAImageWidget);
    this->patientMRAImageConfigurationWidgetLayout->setSpacing(0);
    this->patientMRAImageConfigurationWidgetLayout->setMargin(0);

    //! --------------------------------------------------------------------------------------------------------
    //! patient clinical widgets container(the bigest widget) and its horizonal layout
    //! --------------------------------------------------------------------------------------------------------
    this->patientClinicalWidgetsContainer = new QFrame();
    this->patientClinicalWidgetsContainerLayout = new QHBoxLayout(this->patientClinicalWidgetsContainer);
    this->patientClinicalWidgetsContainerLayout->addWidget(this->patientMRAImageOptionWidget);
    this->patientClinicalWidgetsContainerLayout->addWidget(this->patientMRAImageConfigurationWidget);
    this->patientClinicalWidgetsContainerLayout->setSpacing(3);
    this->patientClinicalWidgetsContainerLayout->setMargin(1);
}

//! -----------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::constructCprAnalyseWidget
//!
void SurgeryPlanWindow::constructTridimensionalImageAnalysingArea(){
    centerlineTreeWidget = new QTreeWidget();
    QString centerlineTreeWidgetStyle = "QTreeWidget{show-decoration-selected:2; background-color:transparent; color:orange; border: 0px solid orange;}" \
                                        "QTreeWidget::item {border: 1px solid #d9d9d9;border-top-color: transparent;border-bottom-color: transparent;}" \
                                        "QTreeWidget::item:hover {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1),border: 1px solid #bfcde4;}" \
                                        "QTreeWidget::item:selected {border: 1px solid #567dbc;}" \
                                        "QTreeWidget::item:selected:active{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1, stop: 1 #567dbc)}" \
                                        "QTreeWidget::item:selected:!active {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6b9be8, stop: 1 #577fbf)}";

    centerlineTreeWidget->setStyleSheet(centerlineTreeWidgetStyle);
    centerlineTreeWidget->setHeaderHidden(1);
    centerlineTreeWidget->setFixedSize(appWidth*0.2,appHeight*0.27);
    centerlineTreeWidget->setStyleSheet("background-color:transparent");
    centerlineTreeWidget->setColumnCount(1);
    centerlineTreeWidget->setHeaderLabel(tr("Centerline Choose"));
    centerlineTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    vesselsFolder = new QTreeWidgetItem(centerlineTreeWidget);
    vesselsFolder->setText(0, "centerlines");
    vesselsFolder->setFlags(Qt::ItemIsEnabled);
    vesselsFolder->setIcon(0, *this->defaultFolderIcon);
    vesselsFolder->setFlags(vesselsFolder->flags()| Qt::ItemIsUserCheckable);
    vesselsFolder->setCheckState(0, Qt::Unchecked);
    centerlineTreeWidget->expandAll();

    //! TODO centerlines
    QString comboBoxStyle = "QComboBox {border: 0px solid gray;border-radius: 3px;padding: 1px 18px 1px 3px;min-width: 6em;}"
                            "QComboBox::drop-down { subcontrol-origin: padding;subcontrol-position: top right;width: 15px;border-left-width: 1px; border-left-color: lightGray; border-left-style: solid;border-top-right-radius: 3px; border-bottom-right-radius: 3px;}";
    centreLineCoordinates = new QComboBox();
    centreLineCoordinates->setStyleSheet(comboBoxStyle);
    centreLineCoordinates->setFont(QFont("Helvetica", 6, QFont::AnyStyle, false));
    centreLineCoordinates->setFixedSize(appWidth*0.1,appHeight*0.02);

    centrelineSlider = new QSlider(Qt::Horizontal);
    centrelineSlider->setFixedSize(appWidth*0.15,appHeight*0.02);

    centrelineParametorArea = new QLabel();

    centrelineParametorArea->setFixedSize(appWidth*0.25,appHeight*0.02);
    centrelineParametorAreaLayout = new QHBoxLayout(centrelineParametorArea);
    centrelineParametorAreaLayout->addWidget(centreLineCoordinates);
    centrelineParametorAreaLayout->addWidget(centrelineSlider);
    centrelineParametorAreaLayout->setSpacing(0);
    centrelineParametorAreaLayout->setMargin(0);

    centerLineVTKWidget = new QVTKWidget();
    centerLineVTKWidget->setFixedSize(appWidth*0.25,appHeight*0.25);

    centrelineVisualiseArea = new QWidget();
    centrelineVisualiseArea->setStyleSheet("background-color:transparent; color:orange; border: 0px solid orange;");
    centrelineVisualiseArea->setFixedSize(appWidth*0.25,appHeight*0.27);
    centrelineVisualiseAreaLayout = new QVBoxLayout(centrelineVisualiseArea);
    centrelineVisualiseAreaLayout->addWidget(centrelineParametorArea);
    centrelineVisualiseAreaLayout->addWidget(centerLineVTKWidget);
    centrelineVisualiseAreaLayout->setSpacing(0);
    centrelineVisualiseAreaLayout->setMargin(0);

    cprOutcomingVTKWidget = new QVTKWidget();
    cprOutcomingVTKWidget->setStyleSheet("background-color:transparent; color:orange; border: 0px solid orange;");
    cprOutcomingVTKWidget->setFixedSize(appWidth*0.25,appHeight*0.27);

    flyThroughWidget = new QWidget();
    flyThroughWidget->setStyleSheet("background-color:transparent; color:orange; border: 0px solid orange;");
    flyThroughWidget->setFixedSize(appWidth*0.3,appHeight*0.27);

    imageAnalyseArea =  new QFrame();
    imageAnalyseArea->setFixedHeight(int(appHeight*0.27));
    imageAnalyseArea->setStyleSheet("background-color:transparent; color:orange; border-top: 1px solid orange;border-radius: 0px;padding: 0 8px; selection-background-color: darkorange");
    cprAnalyseWidgetLayout = new QHBoxLayout(imageAnalyseArea);
    cprAnalyseWidgetLayout->addWidget(centerlineTreeWidget);
    cprAnalyseWidgetLayout->addWidget(centrelineVisualiseArea);
    cprAnalyseWidgetLayout->addWidget(cprOutcomingVTKWidget);
    cprAnalyseWidgetLayout->addWidget(flyThroughWidget);
    cprAnalyseWidgetLayout->setMargin(0);
    cprAnalyseWidgetLayout->setSpacing(0);
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::showSlice
//! \param position
//!
void SurgeryPlanWindow::showSlice(int position){


}


//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::displayCpr
//! \param input
//!
void SurgeryPlanWindow::displayCpr(){
    cprMath();
    cprmapper->SetInputConnection(pCut->GetOutputPort());
    cpractor->SetMapper(cprmapper);
    cprRenderer->AddActor(cpractor);
    cprRenderwindow->AddRenderer(cprRenderer);
    cprOutcomingVTKWidget->SetRenderWindow(cprRenderwindow);
    cprOutcomingVTKWidget->update();
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::sfunctionSource
//!
void SurgeryPlanWindow::sfunctionSource(){
    xSpline = vtkSCurveSpline::New();
    ySpline = vtkSCurveSpline::New();
    zSpline = vtkSCurveSpline::New();
    spline = vtkParametricSpline::New();
    functionSource = vtkParametricFunctionSource::New();

    reslicer = vtkSplineDrivenImageSlicer::New();
    append = vtkImageAppend::New();
    cprmapper1 = vtkFixedPointVolumeRayCastMapper::New();
    cprcolorTranFun = vtkColorTransferFunction::New();
    cprVolumeproperty = vtkVolumeProperty::New();
    cprPieceFun = vtkPiecewiseFunction::New();
    pPlane = vtkPlane::New();
    pCut = vtkCutter::New();
    m_pShift = vtkImageShiftScale::New();
    cprvolume = vtkVolume::New();

    spline->SetXSpline(xSpline);
    spline->SetYSpline(ySpline);
    spline->SetZSpline(zSpline);
    spline->SetPoints(singleVesselPoints);
    functionSource->SetParametricFunction(spline);
    functionSource->SetUResolution(200);
    functionSource->SetVResolution(200);
    functionSource->SetWResolution(200);
    functionSource->Update();
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::cprMath
//!
void SurgeryPlanWindow::cprMath(){

    sfunctionSource();
    reslicer->SetInputData(patientHandling->getMraImageToBeDisplayed());
    reslicer->SetPathConnection(functionSource->GetOutputPort());
    reslicer->SetSliceSpacing( 0.2,0.1 );
    reslicer->SetSliceThickness( 0.8 );
    reslicer->SetSliceExtent(200,200 );
    reslicer->SetOffsetPoint( 30 );
    int nbPoints = functionSource->GetOutput()->GetNumberOfPoints();
    for( int ptId = 0; ptId < nbPoints; ptId++)
    {
        reslicer->SetOffsetPoint(ptId);
        reslicer->Update();
         vtkImageData *tempSlice = vtkImageData::New();
        tempSlice->DeepCopy( reslicer->GetOutput( 0 ));
        append->AddInputData(tempSlice);
    }
    append->SetAppendAxis(2);
    append->Update();

    cprmapper1->SetInputConnection(append->GetOutputPort());
    cprcolorTranFun->AddRGBSegment(0,1,1,1,255,1,1,1);
    cprPieceFun->AddSegment(0,0,3000,1);
    cprPieceFun->AddPoint(20,0.2);
    cprPieceFun->AddPoint(80,0.5);
    cprPieceFun->AddPoint(120,0.7);
    cprPieceFun->AddPoint(200,0.9);
    cprPieceFun->ClampingOff();
    cprmapper1->SetBlendModeToMaximumIntensity();
    cprVolumeproperty->SetColor(cprcolorTranFun);
    cprVolumeproperty->SetScalarOpacity(cprPieceFun);
    cprVolumeproperty->SetInterpolationTypeToLinear();
    cprVolumeproperty->ShadeOff();
    cprvolume->SetProperty(cprVolumeproperty);
    cprvolume->SetMapper(cprmapper1);

    double range[2];
    patientHandling->getMraImageToBeDisplayed()->GetScalarRange(range);
    m_pShift->SetShift(-1.0*range[0]);
    m_pShift->SetScale(255.0/(range[1]-range[0]));
    m_pShift->SetOutputScalarTypeToUnsignedChar();
    m_pShift->SetInputConnection(append->GetOutputPort());
    m_pShift->ReleaseDataFlagOff();
    m_pShift->Update();

    pPlane->SetOrigin(cprvolume->GetCenter());
    pPlane->SetNormal(1,1,0);
    pCut->SetCutFunction(pPlane);
    pCut->SetInputConnection(m_pShift->GetOutputPort());
    pCut->Update();
}


//!--------------------------------------------------------------------------------------------------------------------------------
//!
//!
//! \brief SurgeryPlanWindow::regroupAllComponents
//!
void SurgeryPlanWindow::regroupAllComponents(){

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(patientClinicalWidgetsContainer);
    mainLayout->addWidget(imageAnalyseArea);
    mainLayout->addWidget(controlBar);
    mainLayout->setSpacing(1);
    mainLayout->setContentsMargins(0,0,0,0);
    imageAnalyseArea->close();
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief CanalyserMainWindow::createRandomVtkImageData
//!
void SurgeryPlanWindow::createRandomVtkImageData(){
    vtkSmartPointer<vtkImageData> image = vtkSmartPointer<vtkImageData>::New();
    image->SetDimensions(448,448,128);
    image->SetSpacing(0.5, 0.5, 0.8);
    image->AllocateScalars(VTK_UNSIGNED_SHORT,1);

    int dims[3];
    image->GetDimensions(dims);
    for(int i = 0; i < dims[0]; i++){
      for(int j = 0; j < dims[1]; j++){
          for(int k = 0; k < dims[2]; k++){
            unsigned short* pixel = static_cast<unsigned short*>(image->GetScalarPointer(i,j,k));
            *pixel = 2000;
          }
        }
      }
     display(image);
}

vtkSmartPointer<vtkActor> SurgeryPlanWindow::generateBoundBox(vtkImageData*img){
    vtkSmartPointer<vtkOutlineFilter> filter = vtkSmartPointer<vtkOutlineFilter>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

    filter->SetInputData(img);
    filter->Update();
    mapper->SetInputConnection(filter->GetOutputPort());
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(255,255,255);
    actor->GetProperty()->SetOpacity(1);

    return actor;

}

//! ----------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::generatePolyDataActorFrom
//! \param input
//! \param r
//! \param g
//! \param b
//! \param iterations
//! \return
//!
vtkSmartPointer<vtkActor> SurgeryPlanWindow::generatePolyDataActorFrom(vtkImageData* input, int r, int g, int b,int iterations){
    vtkSmartPointer<vtkMarchingCubes> MC = vtkSmartPointer<vtkMarchingCubes>::New();
    vtkSmartPointer<vtkDecimatePro> deci = vtkSmartPointer<vtkDecimatePro>::New();
    vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    vtkSmartPointer<vtkPolyDataMapper> MC_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> MC_actor = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();

    MC->SetInputData(input);
    MC->SetValue(0, 1);

    deci->SetInputConnection(MC->GetOutputPort());
    deci->SetTargetReduction(1.0);
    deci->PreserveTopologyOn();

    smooth->SetInputConnection(deci->GetOutputPort());
    smooth->SetNumberOfIterations(iterations);
    smooth->BoundarySmoothingOn();

    normals->SetInputConnection(smooth->GetOutputPort());
    normals->FlipNormalsOn();

    writer->SetFileName("C:\\data\\phantom.stl");
    writer->SetInputConnection(normals->GetOutputPort());
    writer->Write();

    MC_mapper->SetScalarVisibility(0);
    MC_mapper->SetInputConnection(normals->GetOutputPort());

    MC_actor->SetMapper(MC_mapper);
    MC_actor->GetProperty()->SetColor(r*1.0/255, g*1.0/255, b*1.0/255);
    //MC_actor->GetProperty()->SetRepresentationToSurface();
    MC_actor->GetProperty()->SetRepresentationToPoints();
    MC_actor->GetProperty()->SetOpacity(1.0);

    return MC_actor;

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::generatePolyDataActor
//! \param path
//! \return
//!
vtkSmartPointer<vtkActor> SurgeryPlanWindow::generatePolyDataActorFromMetaFile(QString path, int r, int g, int b,int iterations){
    vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    vtkSmartPointer<vtkMarchingCubes> MC = vtkSmartPointer<vtkMarchingCubes>::New();
    vtkSmartPointer<vtkDecimatePro> deci = vtkSmartPointer<vtkDecimatePro>::New();
    vtkSmartPointer<vtkSmoothPolyDataFilter> smooth = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
    vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    vtkSmartPointer<vtkPolyDataMapper> MC_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> MC_actor = vtkSmartPointer<vtkActor>::New();

    reader->SetFileName(path.toLatin1().data());
    reader->Update();

    MC->SetInputData(reader->GetOutput());
    MC->SetValue(0, 1);

    deci->SetInputConnection(MC->GetOutputPort());
    deci->SetTargetReduction(1.0);
    deci->PreserveTopologyOn();

    smooth->SetInputConnection(deci->GetOutputPort());
    smooth->SetNumberOfIterations(iterations);
    smooth->BoundarySmoothingOn();

    normals->SetInputConnection(smooth->GetOutputPort());
    normals->FlipNormalsOn();

    MC_mapper->SetScalarVisibility(0);
    MC_mapper->SetInputConnection(normals->GetOutputPort());

    MC_actor->SetMapper(MC_mapper);
    MC_actor->GetProperty()->SetColor(r*1.0/255,g*1.0/255,b*1.0/255);
    MC_actor->GetProperty()->SetRepresentationToSurface();
    //MC_actor->GetProperty()->SetRepresentationToWireframe();
    MC_actor->GetProperty()->SetOpacity(0.6);

    return MC_actor;

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::originalOptionHovered
//!
void SurgeryPlanWindow::originalOptionHovered(){
    originalOption->setStyleSheet("border: 1px solid orange;  border-radius: 0px; background-color: gainsboro;  min-width: 0px; color: orange");
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::originalOptionClicked
//!
void SurgeryPlanWindow::originalOptionClicked(){
    originalOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::originalOptionReleased
//!
void SurgeryPlanWindow::originalOptionReleased(){

    originalOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black  "  );
    transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );

    originalOption->setFont(QFont("Segoe UI",10, QFont::DemiBold, true));
    transparentBrainOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
    greyMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    whiteMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    vesselOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    interventionalRouteOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

    imageOptionStates.originalOptionState = true;
    imageOptionStates.transparentBrainOptionState = false;
    imageOptionStates.greyMatterOptionState = false;
    imageOptionStates.whiteMatterOptionState = false;
    imageOptionStates.vesselOptionState = false;
    imageOptionStates.interventionalRouteOptionState = false;

    //this->display...

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::originalOptionLeaved
//!
void SurgeryPlanWindow::originalOptionLeaved(){
    if(imageOptionStates.originalOptionState){
        originalOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black");
    }
    else{
        originalOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange " );
    }

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::transparentBrainOptionHovered
//!
void SurgeryPlanWindow::transparentBrainOptionHovered(){
    transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: gainsboro;  min-width: 0px; color: orange  "  );
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::transparentBrainOptionClicked
//!
void SurgeryPlanWindow::transparentBrainOptionClicked(){
    transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::transparentBrainOptionReleased
//!
void SurgeryPlanWindow::transparentBrainOptionReleased(){

    this->mainRenderer->RemoveAllViewProps();

    originalOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black  "  );
    greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );

    originalOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
    transparentBrainOption->setFont(QFont("Segoe UI",10, QFont::DemiBold, true));
    greyMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    whiteMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    vesselOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    interventionalRouteOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

    imageOptionStates.originalOptionState = false;
    imageOptionStates.transparentBrainOptionState = true;
    imageOptionStates.greyMatterOptionState = false;
    imageOptionStates.whiteMatterOptionState = false;
    imageOptionStates.vesselOptionState = false;
    imageOptionStates.interventionalRouteOptionState = false;

    qDebug()<<"start extractBrainCortextFrom";

    if(this->grayMatterActor == NULL){
        QString gmPath = this->patientHandling->getTridimensionelPath() + this->patientHandling->getLastName() + "__" +this->patientHandling->getFirstName() + "_gray_matter.mha";
        this->grayMatterActor = generatePolyDataActorFromMetaFile(gmPath, 105, 105, 105, 1000);
    }

    if(this->whiteMatterActor == NULL){
        QString wmPath = this->patientHandling->getTridimensionelPath() + this->patientHandling->getLastName() + "__" +this->patientHandling->getFirstName() + "_white_matter.mha";
        this->whiteMatterActor = generatePolyDataActorFromMetaFile(wmPath, 255, 255, 255, 1000);
    }

    //! reconstruct vascular vessel
    /*if(this->vesselLeftActor == NULL){
        QString vesselString = this->patientHandling->getTridimensionelPath() + this->patientHandling->getLastName() + "__" +this->patientHandling->getFirstName();
        //! QString vesselPath = this->patientHandling->getTridimensionelPath() + this->patientHandling->getLastName() + "__" +this->patientHandling->getFirstName() + "_vessel.mha";
        QString vesselLeftPath = vesselString + "_vesselLeft.mha";
        QString vesselRight = vesselString + "_vesselRight.mha";
        QString vesselFrontPath = vesselString + "_vesselFront.mha";
        QString vesselBackPath = vesselString + "_vesselBack.mha";

        qDebug()<<vesselLeftPath;
        qDebug()<<vesselRight;
        qDebug()<<vesselFrontPath;
        qDebug()<<vesselBackPath;

        QFileInfo checkResultFile(vesselLeftPath);

        //! check if the target file exists and if yes: Is it really a file and no directory?
        if((checkResultFile.exists() && checkResultFile.isFile()) ){
            qDebug()<<"fichier existe deja";
            this->vesselLeftActor = generatePolyDataActorFromMetaFile(vesselLeftPath, 204, 0, 0, 200);
            this->vesselRightActor = generatePolyDataActorFromMetaFile(vesselRight, 204, 0, 0, 200);
            this->vesselBackActor = generatePolyDataActorFromMetaFile(vesselBackPath, 204, 255, 153, 200);
            this->vesselFrontActor = generatePolyDataActorFromMetaFile(vesselFrontPath, 255, 255, 51, 200);
        }
        else{
            //! need to call vessel segmentation algo
        }
    }

    this->mainRenderer->AddActor(this->vesselLeftActor);
    this->mainRenderer->AddActor(this->vesselRightActor);
    this->mainRenderer->AddActor(this->vesselBackActor);
    this->mainRenderer->AddActor(this->vesselFrontActor);*/


    this->mainRenderer->AddActor(this->grayMatterActor);
    this->mainRenderer->AddActor(this->whiteMatterActor);

//    this->mainRenderer->ResetCamera();
//    this->renderWindow->Render();
    this->patientMRAImage->update();

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::transparentBrainOptionLeaved
//!
void SurgeryPlanWindow::transparentBrainOptionLeaved(){
    if(imageOptionStates.transparentBrainOptionState){
        transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black");
    }
    else{
        transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange " );
    }
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::greyMatterOptionHovered
//!
void SurgeryPlanWindow::greyMatterOptionHovered(){
    greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: gainsboro;  min-width: 0px; color: orange  "  );
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::greyMatterOptionClicked
//!
void SurgeryPlanWindow::greyMatterOptionClicked(){
    greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::greyMatterOptionReleased
//!
void SurgeryPlanWindow::greyMatterOptionReleased(){

    this->mainRenderer->RemoveAllViewProps();

    originalOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black  "  );
    whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );

    originalOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
    transparentBrainOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
    greyMatterOption->setFont(QFont("Segoe UI", 10, QFont::DemiBold, true));
    whiteMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    vesselOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    interventionalRouteOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

    imageOptionStates.originalOptionState = false;
    imageOptionStates.transparentBrainOptionState = false;
    imageOptionStates.greyMatterOptionState = true;
    imageOptionStates.whiteMatterOptionState = false;
    imageOptionStates.vesselOptionState = false;
    imageOptionStates.interventionalRouteOptionState = false;

    qDebug()<<"start extractBrainCortext gray matter";

    if(this->grayMatterActor == NULL){
        QString gmPath = this->patientHandling->getTridimensionelPath() + this->patientHandling->getLastName() + "__" +this->patientHandling->getFirstName() + "_gray_matter.mha";
        QFileInfo checkResultFile(gmPath);

        //! check if the target file exists and if yes: Is it really a file and no directory?
        if((checkResultFile.exists() && checkResultFile.isFile()) ){
            qDebug()<<"fichier existe deja";
        }
        else{
            if(!this->systemDispatcher->extractBrainCortextFrom(this->patientHandling->getMraImageToBeDisplayed(), "gm", gmPath)){
                return;
            }
        }
        this->grayMatterActor = generatePolyDataActorFromMetaFile(gmPath, 105, 105, 105, 1000);
    }

    this->mainRenderer->AddActor(this->grayMatterActor);

    this->mainRenderer->ResetCamera();
    this->renderWindow->Render();
    this->patientMRAImage->update();

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::greyMatterOptionLeaved
//!
void SurgeryPlanWindow::greyMatterOptionLeaved(){
    if(imageOptionStates.greyMatterOptionState){
        greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black");
    }
    else{
        greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange " );
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::whiteMatterOptionHovered
//!
void SurgeryPlanWindow::whiteMatterOptionHovered(){
    whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: gainsboro;  min-width: 0px; color: orange  "  );
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::whiteMatterOptionClicked
//!
void SurgeryPlanWindow::whiteMatterOptionClicked(){
    whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::whiteMatterOptionReleased
//!
void SurgeryPlanWindow::whiteMatterOptionReleased(){

    this->mainRenderer->RemoveAllViewProps();

    originalOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black  "  );
    vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );

    originalOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
    transparentBrainOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
    greyMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    whiteMatterOption->setFont(QFont("Segoe UI", 10, QFont::DemiBold, true));
    vesselOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    interventionalRouteOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

    imageOptionStates.originalOptionState = false;
    imageOptionStates.transparentBrainOptionState = false;
    imageOptionStates.greyMatterOptionState = false;
    imageOptionStates.whiteMatterOptionState = true;
    imageOptionStates.vesselOptionState = false;
    imageOptionStates.interventionalRouteOptionState = false;

    qDebug()<<"start extractBrainCortext white matter";


    if(this->whiteMatterActor == NULL){
        QString wmPath = this->patientHandling->getTridimensionelPath() + this->patientHandling->getLastName() + "__" +this->patientHandling->getFirstName() + "_white_matter.mha";
        QFileInfo checkResultFile(wmPath);

        //! check if the target file exists and if yes: Is it really a file and no directory?
        if((checkResultFile.exists() && checkResultFile.isFile()) ){
            qDebug()<<"fichier existe deja";
        }
        else{
            if(!this->systemDispatcher->extractBrainCortextFrom(this->patientHandling->getMraImageToBeDisplayed(), "wm",wmPath)){
                return;
            }
        }

        this->whiteMatterActor = generatePolyDataActorFromMetaFile(wmPath, 255, 255, 255, 1000);
    }

    this->mainRenderer->AddActor(this->whiteMatterActor);

    this->mainRenderer->ResetCamera();
    this->renderWindow->Render();
    this->patientMRAImage->update();
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::whiteMatterOptionLeaved
//!
void SurgeryPlanWindow::whiteMatterOptionLeaved(){
    if(imageOptionStates.whiteMatterOptionState){
        whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black");
    }
    else{
        whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange " );
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::vesselOptionHovered
//!
void SurgeryPlanWindow::vesselOptionHovered(){
    vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: gainsboro;  min-width: 0px; color: orange  "  );
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::vesselOptionClicked
//!
void SurgeryPlanWindow::vesselOptionClicked(){
    vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
}


//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::vesselOptionReleased
//!
void SurgeryPlanWindow::vesselOptionReleased(){
    //! clear all component on the qvtkwidget
    this->mainRenderer->RemoveAllViewProps();

    //! embelleshment
    originalOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black  "  );
    interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );

    originalOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
    transparentBrainOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
    greyMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    whiteMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    vesselOption->setFont(QFont("Segoe UI", 10, QFont::DemiBold, true));
    interventionalRouteOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

    imageOptionStates.originalOptionState = false;
    imageOptionStates.transparentBrainOptionState = false;
    imageOptionStates.greyMatterOptionState = false;
    imageOptionStates.whiteMatterOptionState = false;
    imageOptionStates.vesselOptionState = true;
    imageOptionStates.interventionalRouteOptionState = false;

    //! reconstruct vascular vessel
    /*if(this->vesselLeftActor == NULL){
        QString vesselString = this->patientHandling->getTridimensionelPath() + this->patientHandling->getLastName() + "__" +this->patientHandling->getFirstName();
        //! QString vesselPath = this->patientHandling->getTridimensionelPath() + this->patientHandling->getLastName() + "__" +this->patientHandling->getFirstName() + "_vessel.mha";
        QString vesselLeftPath = vesselString + "_vesselLeft.mha";
        QString vesselRight = vesselString + "_vesselRight.mha";
        QString vesselFrontPath = vesselString + "_vesselFront.mha";
        QString vesselBackPath = vesselString + "_vesselBack.mha";

        qDebug()<<vesselLeftPath;
        qDebug()<<vesselRight;
        qDebug()<<vesselFrontPath;
        qDebug()<<vesselBackPath;

        QFileInfo checkResultFile(vesselLeftPath);

        //! check if the target file exists and if yes: Is it really a file and no directory?
        if((checkResultFile.exists() && checkResultFile.isFile()) ){
            qDebug()<<"fichier existe deja";
            this->vesselLeftActor = generatePolyDataActorFromMetaFile(vesselLeftPath, 204, 0, 0, 200);
            this->vesselRightActor = generatePolyDataActorFromMetaFile(vesselRight, 204, 0, 0, 200);
            this->vesselBackActor = generatePolyDataActorFromMetaFile(vesselBackPath, 204, 255, 153, 200);
            this->vesselFrontActor = generatePolyDataActorFromMetaFile(vesselFrontPath, 255, 255, 51, 200);
        }
        else{
            //! need to call vessel segmentation algo
        }
    }

    this->mainRenderer->AddActor(this->vesselLeftActor);
    this->mainRenderer->AddActor(this->vesselRightActor);
    this->mainRenderer->AddActor(this->vesselBackActor);
    this->mainRenderer->AddActor(this->vesselFrontActor);

    this->mainRenderer->ResetCamera();
    this->renderWindow->Render();
    this->patientMRAImage->update();*/

    //QList<QString> vesselActorsKeys = vesselActors.keys();
    //QList<QString> cheminActorsKeys = cheminActors.keys();

//    for(int i = 0; i < vesselActorsKeys.length(); i++){
//         this->mainRenderer->AddActor(vesselActors[vesselActorsKeys.at(i)]);
//        this->mainRenderer->AddActor(cheminActors[vesselActorsKeys.at(i)]);
//    }
//    this->patientMRAImage->update();

    vesselActor = generatePolyDataActorFrom(this->patientHandling->getMraImageToBeDisplayed(), 255,128,0, 250);
    this->mainRenderer->AddActor(vesselActor);
    this->patientMRAImage->update();
}


void SurgeryPlanWindow::getVesselEnhancedImage(){
    //qDebug()<<enhancedImage->GetDataDimension();
}


//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::vesselOptionLeaved
//!
void SurgeryPlanWindow::vesselOptionLeaved(){
    if(imageOptionStates.vesselOptionState){
        vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black");
    }
    else{
        vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange " );
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::interventionalRouteOptionHovered
//!
void SurgeryPlanWindow::interventionalRouteOptionHovered(){
     interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: gainsboro;  min-width: 0px; color: orange  "  );
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::interventionalRouteOptionClicked
//!
void SurgeryPlanWindow::interventionalRouteOptionClicked(){
     interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::interventionalRouteOptionReleased
//!
void SurgeryPlanWindow::interventionalRouteOptionReleased(){
    this->mainRenderer->RemoveAllViewProps();
    originalOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    transparentBrainOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    greyMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    whiteMatterOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    vesselOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange  "  );
    interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black  "  );

    originalOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
    transparentBrainOption->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
    greyMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    whiteMatterOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    vesselOption->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
    interventionalRouteOption->setFont(QFont("Segoe UI", 10, QFont::DemiBold, true));

    imageOptionStates.originalOptionState = false;
    imageOptionStates.transparentBrainOptionState = false;
    imageOptionStates.greyMatterOptionState = false;
    imageOptionStates.whiteMatterOptionState = false;
    imageOptionStates.vesselOptionState = false;
    imageOptionStates.interventionalRouteOptionState = true;
    //displayCenterLine();
    //reconstructButtonClicked();
    QString gmPath = this->patientHandling->getTridimensionelPath() + this->patientHandling->getLastName() + "__" +this->patientHandling->getFirstName() + "_vessel.mha";
    this->grayMatterActor = generatePolyDataActorFromMetaFile(gmPath, 204, 105, 50, 1000);

    this->mainRenderer->AddActor(this->grayMatterActor);

    this->mainRenderer->ResetCamera();
    this->renderWindow->Render();
    this->patientMRAImage->update();

}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::interventionalRouteOptionLeaved
//!
void SurgeryPlanWindow::interventionalRouteOptionLeaved(){
    if(imageOptionStates.interventionalRouteOptionState){
        interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: orange;  min-width: 0px; color: black");
    }
    else{
        interventionalRouteOption->setStyleSheet( "border: 1px solid orange;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: orange " );
    }
}


//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::displayConfigurationBoard
//!
void SurgeryPlanWindow::displayConfigurationBoard(){
    this->patientWidgetConfigurationBoard->display(QCursor::pos());
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::displayCurveReformatwionWindow
//!
void SurgeryPlanWindow::displayImageAnalyseArea(){
    displayImageAnalyseAreaButtonClicked = !displayImageAnalyseAreaButtonClicked;
    if(displayImageAnalyseAreaButtonClicked){
        dislayImageAnalyseAreaButton->setIcon(QIcon(":/images/pulldown.png"));
        imageAnalyseArea->show();
    }
    else{
        dislayImageAnalyseAreaButton->setIcon(QIcon(":/images/pushup.png"));
        imageAnalyseArea->close();
    }

}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::closeSurgeryPlanWindow
//!
void SurgeryPlanWindow::closeSurgeryPlanWindow(){
    //this->close();
    //this->display(this->patientHandling->getMraImageToBeDisplayed());
}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::constructIHM
//!
void SurgeryPlanWindow::constructIHM(){

    this->constructTridimensionalImageVisualisingArea();

    this->constructTridimensionalImageAnalysingArea();

    this->constructControlBar();

    this->regroupAllComponents();

}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::drawBackground
//!
void SurgeryPlanWindow::drawBackground(){
    this->setStyleSheet("background:"+this->globalWorkSpaceColor);

    setWorkSpaceColor(this->globalWorkSpaceColor);

//    QPixmap * pixmap = new QPixmap(":/images/papier.png");
//    QPalette palette = this->palette();
//    palette.setBrush(QPalette::Background, QBrush(pixmap->scaled(QSize(this->appWidth, this->appHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
//    this->setPalette(palette);
//    this->setMask(pixmap->mask());

//    this->setAutoFillBackground(true);

}

//! --------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief SurgeryPlanWindow::setConnections
//!
void SurgeryPlanWindow::setConnections(){

    // switch the volume rendering options
    this->connect(opacityTransferChoice, SIGNAL(toggled(bool)), this, SLOT(opacityTransformationStateChanged(bool)));
    this->connect(colorTransferChoice, SIGNAL(toggled(bool)), this, SLOT(colorTransformationStateChanged(bool)));
    this->connect(gradientTransferChoice, SIGNAL(toggled(bool)), this, SLOT(gradientTransformationStateChanged(bool)));

    // actions of the mouse's pointer on the plotting board
    this->connect(transformationPlottingBoard, SIGNAL(mouseDoubleClick(QMouseEvent*)), this, SLOT(generateNewTransformationPoint(QMouseEvent*)));
    this->connect(transformationPlottingBoard, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(transformPointTracking(QMouseEvent*)));
    this->connect(transformationPlottingBoard, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(lockTargetPoint(QMouseEvent*)));
    this->connect(transformationPlottingBoard, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(positioningTransformPoint(QMouseEvent*)));

    // ....
    this->connect(this->originalOption, SIGNAL(mouseHover()), this, SLOT(originalOptionHovered()));
    this->connect(this->originalOption, SIGNAL(mouseLeftButtonClicked()), this, SLOT(originalOptionClicked()));
    this->connect(this->originalOption, SIGNAL(mouseLeftButtonReleased()), this, SLOT(originalOptionReleased()));
    this->connect(this->originalOption, SIGNAL(mouseLeaved()), this, SLOT(originalOptionLeaved()));

    this->connect(this->transparentBrainOption, SIGNAL(mouseHover()), this, SLOT(transparentBrainOptionHovered()));
    this->connect(this->transparentBrainOption, SIGNAL(mouseLeftButtonClicked()), this, SLOT(transparentBrainOptionClicked()));
    this->connect(this->transparentBrainOption, SIGNAL(mouseLeftButtonReleased()), this, SLOT(transparentBrainOptionReleased()));
    this->connect(this->transparentBrainOption, SIGNAL(mouseLeaved()), this, SLOT(transparentBrainOptionLeaved()));

    this->connect(this->greyMatterOption, SIGNAL(mouseHover()), this, SLOT(greyMatterOptionHovered()));
    this->connect(this->greyMatterOption, SIGNAL(mouseLeftButtonClicked()), this, SLOT(greyMatterOptionClicked()));
    this->connect(this->greyMatterOption, SIGNAL(mouseLeftButtonReleased()), this, SLOT(greyMatterOptionReleased()));
    this->connect(this->greyMatterOption, SIGNAL(mouseLeaved()), this, SLOT(greyMatterOptionLeaved()));

    this->connect(this->whiteMatterOption, SIGNAL(mouseHover()), this, SLOT(whiteMatterOptionHovered()));
    this->connect(this->whiteMatterOption, SIGNAL(mouseLeftButtonClicked()), this, SLOT(whiteMatterOptionClicked()));
    this->connect(this->whiteMatterOption, SIGNAL(mouseLeftButtonReleased()), this, SLOT(whiteMatterOptionReleased()));
    this->connect(this->whiteMatterOption, SIGNAL(mouseLeaved()), this, SLOT(whiteMatterOptionLeaved()));

    this->connect(this->vesselOption, SIGNAL(mouseHover()), this, SLOT(vesselOptionHovered()));
    this->connect(this->vesselOption, SIGNAL(mouseLeftButtonClicked()), this, SLOT(vesselOptionClicked()));
    this->connect(this->vesselOption, SIGNAL(mouseLeftButtonReleased()), this, SLOT(vesselOptionReleased()));
    this->connect(this->vesselOption, SIGNAL(mouseLeaved()), this, SLOT(vesselOptionLeaved()));

    this->connect(this->interventionalRouteOption, SIGNAL(mouseHover()), this, SLOT(interventionalRouteOptionHovered()));
    this->connect(this->interventionalRouteOption, SIGNAL(mouseLeftButtonClicked()), this, SLOT(interventionalRouteOptionClicked()));
    this->connect(this->interventionalRouteOption, SIGNAL(mouseLeftButtonReleased()), this, SLOT(interventionalRouteOptionReleased()));
    this->connect(this->interventionalRouteOption, SIGNAL(mouseLeaved()), this, SLOT(interventionalRouteOptionLeaved()));

    this->connect(this->imageConfigurationButton, SIGNAL(mouseLeftButtonClicked()), this, SLOT(displayConfigurationBoard()));
    //this->connect(this->sugeryEndnessButton, SIGNAL(clicked()), this, SLOT(stopSurgery()));
    this->connect(this->dislayImageAnalyseAreaButton,SIGNAL(mouseLeftButtonClicked()),this,SLOT(displayImageAnalyseArea()));
    this->connect(this->quitSurgeryPlanButton, SIGNAL(clicked()), this, SLOT(closeSurgeryPlanWindow()));
    this->connect(this->timerToWaittingReconstructedResult, SIGNAL(timeout()), this, SLOT(toFindReconstructedResult()));
    this->connect(centerlineTreeWidget, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(showContextMenu(const QPoint &)));

    this->connect(this->realTimeTrackingNDITimer, SIGNAL(timeout()), this, SLOT(updateNDIPos()));


    //this->MraConnections = vtkEventQtSlotConnect::New();

    //! update coords as we move through the window
    //this->MraConnections->Connect(patientMRAImage->GetRenderWindow()->GetInteractor(), vtkCommand::MouseMoveEvent, this, SLOT(updateCoords(vtkObject*)));
    //this->MraConnections->Connect(patientMRAImage->GetRenderWindow()->GetInteractor(), vtkCommand::LeftButtonPressEvent, this, SLOT(onImageMouseLeftButtonPressed(vtkObject*)));
    //this->MraConnections->Connect(patientMRAImage->GetRenderWindow()->GetInteractor(), vtkCommand::LeftButtonReleaseEvent, this, SLOT(onImageMouseLeftButtonReleased(vtkObject*)));
}

