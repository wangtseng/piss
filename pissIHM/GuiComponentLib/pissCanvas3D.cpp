#include "pissCanvas3D.h"

pissCanvas3D::pissCanvas3D(int w, int h, QString workspaceColor)
{
    this->setFixedSize(w,h);

    renderer = vtkRenderer::New();
    GetRenderWindow()->AddRenderer(renderer);

    renderer->AddActor(objFileReader("C:\\Users\\cheng\\Documents\\CanalyserWorkspace\\CanalyserImageCenter\\robot\\jxb.obj", 255, 128, 0));
    renderer->AddActor(objFileReader("C:\\Users\\cheng\\Documents\\CanalyserWorkspace\\CanalyserImageCenter\\robot\\tjjg.obj", 255, 155, 0));
    renderer->AddActor(objFileReader("C:\\Users\\cheng\\Documents\\CanalyserWorkspace\\CanalyserImageCenter\\robot\\tjq.obj", 255, 175, 0));
    renderer->AddActor(objFileReader("C:\\Users\\cheng\\Documents\\CanalyserWorkspace\\CanalyserImageCenter\\robot\\xc.obj", 255, 195, 0));
    renderer->AddActor(objFileReader("C:\\Users\\cheng\\Documents\\CanalyserWorkspace\\CanalyserImageCenter\\robot\\xcjg.obj", 255, 215, 0));
    renderer->AddActor(objFileReader("C:\\Users\\cheng\\Documents\\CanalyserWorkspace\\CanalyserImageCenter\\robot\\xcwk.obj", 255, 235, 0));
    renderer->AddActor(objFileReader("C:\\Users\\cheng\\Documents\\CanalyserWorkspace\\CanalyserImageCenter\\robot\\wk.obj", 255, 255, 0));

    //renderer->AddActor(pointsReader("/Users/vincent/Documents/data/Aneurisk/dataset_0001/morphology/centerlines.csv"));
    //renderer->AddActor(reconstructButtonClicked("/Users/vincent/Documents/data/Aneurisk/dataset_0001/morphology/centerlines.csv"));
    //renderer->AddActor(readMetaData("/Users/vincent/Pictures/data/CardiacChallenge/training/dataset00/Artery00AorticMask.mhd"));

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

    renderer->SetBackground((1.0*workspaceRed)/255, (1.0*workspaceGreen)/255, (1.0*workspaceBlue)/255);

    GetRenderWindow()->Render();
}

QVector<CenterLinePoint*> pissCanvas3D::doReadComplteCenterlineFile(QString centerlineFilePath){

    QVector<CenterLinePoint*> vessel;
    QFile file(centerlineFilePath);

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"sorry,can't open the file!"<<endl;
        //return NULL;
    }

    while(!file.atEnd()){
        QByteArray line = file.readLine();
        QString str(line);
        QStringList parameter = str.split(",");
        CenterLinePoint *p = new CenterLinePoint();
        p->set_abscissa(parameter[0].toDouble(0));
        p->set_ordinate(parameter[1].toDouble(0));
        p->set_isometric(parameter[2].toDouble(0));
        p->set_radius(parameter[3].toDouble(0));
        vessel.append(p);
    }
    return vessel;
}

vtkActor* pissCanvas3D::pointsReader(QString path){
    vtkPoints* pts = vtkPoints::New();
    vtkCellArray* vertices = vtkCellArray::New();
    vtkPolyData* polyData = vtkPolyData::New();
    vtkPolyDataMapper *ptsMapper = vtkPolyDataMapper::New();
    vtkActor *ptsActor = vtkActor::New();

    QVector<CenterLinePoint*> vesselPoints = doReadComplteCenterlineFile(path);

    int cpt;
    for(cpt = 0; cpt<vesselPoints.size(); cpt++){
        vtkIdType pid[1];
        pid[0] = pts->InsertNextPoint(vesselPoints.at(cpt)->get_abscissa(),vesselPoints.at(cpt)->get_ordinate(),vesselPoints.at(cpt)->get_isometric());
        vertices->InsertNextCell(1,pid);
    }

    // Add the points to the dataset
    polyData->SetPoints(pts);
    polyData->SetVerts(vertices);

    ptsMapper->SetInputData(polyData);
    ptsActor->SetMapper(ptsMapper);
    ptsActor->GetProperty()->SetColor(0.3800, 0.7000, 0.1600);

    return ptsActor;
}

vtkActor* pissCanvas3D::objFileReader(QString path, int r, int g, int b){
    vtkOBJReader* reader = vtkOBJReader::New();
    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    vtkActor *stlActor = vtkActor::New();

    reader->SetFileName(path.toLocal8Bit().data());
    reader->Update();

    mapper->SetInputConnection(reader->GetOutputPort());

    stlActor->SetMapper(mapper);
    stlActor->GetProperty()->SetOpacity(0.3);
    stlActor->GetProperty()->SetColor(r*1.0/255, g*1.0/255, b*1.0/255);
    stlActor->GetProperty()->SetRepresentationToSurface();

    return stlActor;
}

vtkActor* pissCanvas3D::stlFileReader(QString path){
    vtkSTLReader* reader = vtkSTLReader::New();
    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    vtkActor *stlActor = vtkActor::New();

    reader->SetFileName(path.toLocal8Bit().data());
    reader->Update();

    mapper->SetInputConnection(reader->GetOutputPort());

    stlActor->SetMapper(mapper);
    stlActor->GetProperty()->SetOpacity(0.3);
    stlActor->GetProperty()->SetColor(209.0/255, 54.0/255, 54.0/255);
    stlActor->GetProperty()->SetRepresentationToPoints();

    return stlActor;
}

vtkActor* pissCanvas3D::splineCurveGenerate(QString path){
    vtkPoints* pts = vtkPoints::New();
    vtkParametricSpline *spline = vtkParametricSpline::New();
    vtkParametricFunctionSource *splineSource = vtkParametricFunctionSource::New();
    vtkPolyDataMapper *splineMapper = vtkPolyDataMapper::New();
    vtkActor *splineActor = vtkActor::New();


    QVector<CenterLinePoint*> vesselPoints = doReadComplteCenterlineFile(path);

    int cpt;
    for(cpt = 0; cpt<vesselPoints.size(); cpt++){
        pts->InsertPoint(cpt,vesselPoints.at(cpt)->get_abscissa(),vesselPoints.at(cpt)->get_ordinate(),vesselPoints.at(cpt)->get_isometric());
    }

    spline->SetPoints(pts);
    spline->ClosedOff();
    splineSource->SetParametricFunction(spline);
    splineMapper->SetInputConnection(splineSource->GetOutputPort());
    splineActor->SetMapper(splineMapper);
    splineActor->GetProperty()->SetColor(0.3800, 0.7000, 0.1600);

    return splineActor;
}

vtkActor* pissCanvas3D::reconstructButtonClicked(QString path){
    vtkPoints* vReconstructPoints = vtkPoints::New();
    vtkCellArray* vReconstructLines = vtkCellArray::New();
    vtkPolyData* vReconstructPolyData = vtkPolyData::New();
    vtkDoubleArray* vReconstructRadius = vtkDoubleArray::New();
    vtkTubeFilter* vReconstructTube = vtkTubeFilter::New();
    vtkPolyDataMapper* vReconstructMapper = vtkPolyDataMapper::New();
    vtkActor* vReconstructActor = vtkActor::New();

    QVector<CenterLinePoint*> vesselPoints = doReadComplteCenterlineFile(path);

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
        if(vesselPoints.at(cpt)->get_radius()==0.0){
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
    vReconstructActor->GetProperty()->SetOpacity(0.3);
    vReconstructActor->GetProperty()->SetColor(209.0/255, 54.0/255, 54.0/255);
    return vReconstructActor;
}

vtkActor *pissCanvas3D::readMetaData(QString path){
    vtkMetaImageReader *reader = vtkMetaImageReader::New();
    reader->SetFileName(path.toLocal8Bit().data());
    reader->Update();

    vtkMarchingCubes * MC= vtkMarchingCubes::New();
    MC->SetInputConnection(reader->GetOutputPort());
    MC->SetValue(0,1.0);

    vtkSmoothPolyDataFilter * smooth=vtkSmoothPolyDataFilter::New();
    smooth->SetInputConnection(MC->GetOutputPort());
    smooth->SetNumberOfIterations(1000);
    smooth->BoundarySmoothingOn();

    vtkPolyDataNormals *normals= vtkPolyDataNormals::New();
    normals->SetInputConnection(smooth->GetOutputPort());
    normals->FlipNormalsOn();

    vtkPolyDataMapper * mapper= vtkPolyDataMapper::New();
    mapper->SetInputConnection(normals->GetOutputPort());
    mapper->SetScalarVisibility(0);

    vtkActor * MC_Actor= vtkActor::New();
    MC_Actor->SetMapper(mapper);
    MC_Actor->GetProperty()->SetColor(209.0/255, 54.0/255, 54.0/255);
    MC_Actor->GetProperty()->SetRepresentationToSurface();

    return MC_Actor;
}

