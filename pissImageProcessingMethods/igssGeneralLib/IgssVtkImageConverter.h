#ifndef IGSSVTKIMAGECONVERTER_H
#define IGSSVTKIMAGECONVERTER_H

#include <vtkImageData.h>
#include <IgssImage.h>
#include <vtkSmartPointer.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>

class IgssVtkImageConverter{

public:
    IgssVtkImageConverter();

    void IgssToVtk(IgssImage* input, vtkSmartPointer<vtkImageData> output);
    void VtkToIgss(vtkSmartPointer<vtkImageData> input, IgssImage *output);
};

#endif // IGSSVTKIMAGECONVERTER_H
