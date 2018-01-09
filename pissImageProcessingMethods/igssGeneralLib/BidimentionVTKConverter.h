#ifndef BIDIMENTIONVTKCONVERTER_H
#define BIDIMENTIONVTKCONVERTER_H

#include <vtkImageData.h>
#include <BidimensionnelImage.h>
#include <vtkSmartPointer.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>

class BidimentionVTKConverter
{
public:
    BidimentionVTKConverter();
    ~BidimentionVTKConverter();

    void BidimensionToVtk(BidimensionnelImage* input, vtkSmartPointer<vtkImageData> output);
    void VtkToBidimention(vtkSmartPointer<vtkImageData> input, BidimensionnelImage *output);
};

#endif // BIDIMENTIONVTKCONVERTER_H
