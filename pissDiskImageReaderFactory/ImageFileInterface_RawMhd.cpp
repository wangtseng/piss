#include "ImageFileInterface_RawMhd.h"


ImageFileInterface_RawMhd::ImageFileInterface_RawMhd(){
    reader = vtkSmartPointer<vtkMetaImageReader>::New();
}

ImageFileInterface_RawMhd::~ImageFileInterface_RawMhd(){

}

//! -------------------------------------------------------------------------------------------------
//!
//! \brief ImageFileInterface_RawMhd::readFrom
//! \param filepath
//! \return
//!
eImageErrorCode ImageFileInterface_RawMhd::readFrom(const QString &filepath){

    eImageErrorCode ret = IMAGE_NO_ERRROR;

    if(reader->CanReadFile(filepath.toLocal8Bit().data()) == 0){
        return IMAGE_MetaFile_ERROR;
    }

    reader->SetFileName(filepath.toLocal8Bit().data());

    reader->Update();

    return(ret);
}

//! ------------------------------------------------------------------------------------------------------
//!
//! \brief ImageFileInterface_RawMhd::getImage
//! \return
//!
vtkImageData * ImageFileInterface_RawMhd::getImage(){
    return reader->GetOutput();
}

void ImageFileInterface_RawMhd::getImageExtentInformation(int* extent){
    reader->GetOutputInformation(0)->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent);
}
