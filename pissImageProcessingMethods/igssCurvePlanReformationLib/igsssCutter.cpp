#include "igsssCutter.h"
#include"vtkPolyDataWriter.h"



igsssCutter::igsssCutter()
{
    plane = vtkPlane::New();
    cutter = vtkCutter::New();
    mapper = vtkPolyDataMapper::New();

}

//! --------------------------------------------------------------------------------------------------------
//!
//! \brief cut
//! \param p0
//! \param p1
//! \param p2
//! \param shiftScaleImageData
//! \return
//!
vtkActor * igsssCutter::cut(double* p0, double* p1, double* p2, vtkImageShiftScale* shiftScaleImageData){

    vtkActor *ret = vtkActor::New();
    //vtkExtractVOI

    plane->SetOrigin(p1);
    plane->SetNormal((p2[0]-p0[0])/2,(p2[1]-p0[1])/2,(p2[2]-p0[2])/2);
    cutter->SetCutFunction(plane);
    cutter->SetInputConnection(shiftScaleImageData->GetOutputPort());
    cutter->Update();
    cutter->GenerateCutScalarsOff();

    mapper->SetInputData(cutter->GetOutput());
    mapper->Update();

    ret->SetMapper(mapper);

    return ret;
}

vtkActor * igsssCutter::cut1(double* p0, double* center, double* p2, vtkImageData* img){
      vtkActor *ret = vtkActor::New();
//    vtkImageReslice* reslice = vtkImageReslice::New();
//    vtkImageData *out = vtkImageData::New();

//    double tangent[3];
//    int cpt;
//    for(cpt = 0; cpt < 3; cpt++){
//        tangent[cpt] = (p2[cpt] - p0[cpt])/2;
//    }

//    double vLast[3];
//    for(cpt = 0; cpt < 3; cpt++){
//        vLast[cpt] = (center[cpt] - p0[cpt]);
//    }

//    double vNext[3];
//    for(cpt = 0; cpt < 3; cpt++){
//        vNext[cpt] = (p2[cpt] - center[cpt]);
//    }

//    double normal[3];
//    vtkMath::Cross(vLast, vNext, normal);

//    double binormal[3];
//    vtkMath::Cross(tangent, normal, binormal);

//    vtkSmartPointer<vtkMatrix4x4> resliceAxes =   vtkSmartPointer<vtkMatrix4x4>::New();

//    resliceAxes->SetElement(0,0, normal[0]);
//    resliceAxes->SetElement(1,0, normal[1]);
//    resliceAxes->SetElement(2,0, normal[2]);
//    resliceAxes->SetElement(3,0, 0);

//    resliceAxes->SetElement(0,1, binormal[0]);
//    resliceAxes->SetElement(1,1, binormal[1]);
//    resliceAxes->SetElement(2,1, binormal[2]);
//    resliceAxes->SetElement(3,1, 0);

//    resliceAxes->SetElement(0,2, tangent[0]);
//    resliceAxes->SetElement(1,2, tangent[1]);
//    resliceAxes->SetElement(2,2, tangent[2]);
//    resliceAxes->SetElement(3,1, 0);

//    resliceAxes->SetElement(0,3, center[0]);
//    resliceAxes->SetElement(1,3, center[1]);
//    resliceAxes->SetElement(2,3, center[2]);
//    resliceAxes->SetElement(3,3, 1);

//    resliceAxes->SetElement(0,3, center[0]);
//    resliceAxes->SetElement(1,3, center[1]);
//    resliceAxes->SetElement(2,3, center[2]);

//    reslice->SetInputData(img);
//    reslice->SetOutputDimensionality(2);
//    reslice->SetResliceAxes(resliceAxes);
//    reslice->SetInterpolationModeToLinear();
//    reslice->SetOutputOrigin(center[0],center[1],center[2]);
//    reslice->SetOutputExtent(0, 100, 0, 100, 0, 1);
//    reslice->SetOutputSpacing(0.5,0.5,3.8);
//    reslice->Update();
//    out->DeepCopy(reslice->GetOutputDataObject(0));

    return ret;
}
