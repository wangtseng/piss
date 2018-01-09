#ifndef IGSSSCUTTER_H
#define IGSSSCUTTER_H

#include "vtkPlane.h"
#include "vtkCutter.h"
#include "vtkActor.h"
#include "vtkImageShiftScale.h"
#include "vtkImageReslice.h"
#include "vtkMath.h"
#include "vtkPolyDataMapper.h"
#include "vtkMatrix4x4.h"
#include "vtkSmartPointer.h"
#include "vtkImageViewer2.h"
#include "vtkImageData.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkDataObject.h"
#include "vtkRenderer.h"
#include "vtkExtractVOI.h"

#include <QDebug>


class igsssCutter{

private:
    vtkPlane *plane;
    vtkCutter *cutter;
    vtkPolyDataMapper *mapper;

public:
    vtkActor * cut(double* p0, double* p1, double* p2, vtkImageShiftScale* shiftScaleImageData);
    vtkActor * cut1(double* p0, double* center, double* p2, vtkImageData* img);

public:
    igsssCutter();
};

#endif // IGSSSCUTTER_H
