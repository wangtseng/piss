#include "ImageFileInterface_Dicomdir.h"

ImageFileInterface_Dicomdir::ImageFileInterface_Dicomdir()
{

}

ImageFileInterface_Dicomdir::~ImageFileInterface_Dicomdir()
{

}

eImageErrorCode ImageFileInterface_Dicomdir::readFrom(const QString &filepath){
    eImageErrorCode ret = IMAGE_NO_ERRROR;
    this->dicomdirFileAnalyser(filepath.toLocal8Bit().data());
    return(ret);
}

vtkImageData* ImageFileInterface_Dicomdir::getImage(){
    vtkImageData* ret = NULL;
    return ret;
}

void ImageFileInterface_Dicomdir::dicomdirFileAnalyser(const char *filepath){

}

void ImageFileInterface_Dicomdir::getImageExtentInformation(int* extent){

}
