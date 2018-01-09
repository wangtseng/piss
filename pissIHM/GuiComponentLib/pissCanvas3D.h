#ifndef PISSCANVAS3D_H
#define PISSCANVAS3D_H

#include <QVector>
#include <QString>
#include <QStringList>
#include <QFile>

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkTubeFilter.h>
#include <vtkDoubleArray.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkVolume.h>
#include <vtkMetaImageReader.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkPolyLine.h>
#include <vtkSTLReader.h>
#include <vtkOBJReader.h>
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkPiecewiseFunction.h"
#include "vtkImageData.h"
#include <QVTKWidget.h>
#include "CenterLinePoint.h"

#include<vtkMarchingCubes.h>
#include<vtkSmoothPolyDataFilter.h>
#include<vtkActor.h>
#include<vtkPolyDataNormals.h>


class pissCanvas3D : public QVTKWidget
{
public:
    pissCanvas3D(int w, int h, QString workspaceColor);

    vtkActor *readMetaData(QString path);
    vtkActor* reconstructButtonClicked(QString path);
    vtkActor* splineCurveGenerate(QString path);
    vtkActor* stlFileReader(QString path);
    vtkActor* objFileReader(QString path, int r, int g, int b);
    vtkActor* pointsReader(QString path);
    vtkRenderer* renderer;
    QVector<CenterLinePoint*> doReadComplteCenterlineFile(QString centerlineFilePath);

private:
    int workspaceRed;
    int workspaceGreen;
    int workspaceBlue;
};

#endif // PISSCANVAS3D_H
