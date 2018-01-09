#include "IgssVtkImageConverter.h"
#include "IgssImage.h"


IgssVtkImageConverter::IgssVtkImageConverter(){

}


//!----------------------------------------------------------------------------------------------
//!
//! \brief IgssVtkImageConverter::IgssToVtk
//! \param input
//! \param output
//! \author du chen\cheng 2015.09.09
//!
void IgssVtkImageConverter::IgssToVtk(IgssImage* input, vtkSmartPointer<vtkImageData> output){

    int indims[3];
    double inspaces[3];
    int numvoxels;

    input->getDimensionSize(indims);
    input->getElementSpacing(inspaces);

    numvoxels = indims[0]*indims[1]*indims[2];//the number of the whole voxels

    output->SetDimensions(indims[0],indims[1],indims[2]);
    output->SetOrigin(0.0,0.0,0.0);
    output->SetSpacing(inspaces[0],inspaces[1],inspaces[2]);

    if(input->getIgssScalarType()==IGSS_UNSIGNED_SHORT){
        output->AllocateScalars(VTK_UNSIGNED_SHORT,1);
    }

    vtkDataArray* output_array = output->GetPointData()->GetScalars();
    //unsigned short *ptr = (unsigned short*)output->GetScalarPointer();
    long index;
    for(index = 0; index < numvoxels; index++){
        //*ptr++ = input->getValueByIndex(index);
        output_array->SetComponent(index, 0, input->getValueByIndex(index));
    }
    //qDebug()<<"IgssToVtk"<<numvoxels<<indims[0]<<indims[1]<<indims[2];
}

//!--------------------------------------------------------------------------------------
//!
//! \brief IgssVtkImageConverter::VtkToIgss
//! \param input vtkImageData
//! \param output IgssImage
//! \author lishuai \date 2015-9-11
//!
void IgssVtkImageConverter::VtkToIgss(vtkSmartPointer<vtkImageData> input, IgssImage *output){
    int nDims[3];
    double spacings[3];
    input->GetDimensions(nDims);
    input->GetSpacing(spacings);

    //!decide the dimensions
    if(nDims[2]!=0)
    {
        output->setNumberOfDimension(3);
    }
    else
    {
        output->setNumberOfDimension(2);
    }

    output->setDimensionSize(nDims[0],nDims[1],nDims[2]);
    output->setElementSpacing(spacings[0],spacings[1],spacings[2]);
    output->setLengthOfDataSet();

    long index = 0;
    if(input->GetScalarType()==VTK_UNSIGNED_SHORT)
    {
        unsigned short* piexl = (unsigned short*)input->GetScalarPointer();
        for(long i=0;i<nDims[2]*nDims[1]*nDims[0];i++)
        {
            output->setValueByIndex(index,*piexl++);
            index++;
        }

    }

}
