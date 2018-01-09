#include "BidimentionVTKConverter.h"

BidimentionVTKConverter::BidimentionVTKConverter()
{

}

BidimentionVTKConverter::~BidimentionVTKConverter()
{

}

//!------------------------------------------------------------------------------------------------------
//!
//! \brief BidimentionVTKConverter::BidimensionToVtk
//! \param input
//! \param output
//!
void BidimentionVTKConverter::BidimensionToVtk(BidimensionnelImage* input, vtkSmartPointer<vtkImageData> output){

}

//    int indims[3];
//    double inspaces[3];
//    int numvoxels;

//    input->getDimensionSize(indims);
//    input->getElementSpacing(inspaces);

//    numvoxels = indims[0]*indims[1]*indims[2];//the number of the whole voxels

//    output->SetDimensions(indims[0],indims[1],indims[2]);
//    output->SetOrigin(0.0,0.0,0.0);
//    output->SetSpacing(inspaces[0],inspaces[1],inspaces[2]);

//    if(input->getIgssScalarType()==IGSS_UNSIGNED_SHORT){
//        output->AllocateScalars(VTK_UNSIGNED_SHORT,1);
//    }

//    vtkDataArray* output_array = output->GetPointData()->GetScalars();
//    //unsigned short *ptr = (unsigned short*)output->GetScalarPointer();
//    long index;
//    for(index = 0; index < numvoxels; index++){
//        //*ptr++ = input->getValueByIndex(index);
//        output_array->SetComponent(index, 0, input->getValueByIndex(index));
//    }
//    qDebug()<<"IgssToVtk"<<numvoxels<<indims[0]<<indims[1]<<indims[2];
//}
