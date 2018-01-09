#include "ImageFileInterface_Mat.h"

ImageFileInterface_Mat::ImageFileInterface_Mat(){
    reader = vtkSmartPointer<vtkMetaImageReader>::New();
}

ImageFileInterface_Mat::~ImageFileInterface_Mat()
{

}

eImageErrorCode ImageFileInterface_Mat::readFrom(const QString &filepath){
    // vtkImageData
    eImageErrorCode ret = IMAGE_NO_ERRROR;



return ret;
}

vtkImageData * ImageFileInterface_Mat::getImage(){
    return image;
}

void ImageFileInterface_Mat::getImageExtentInformation(int* extent){
}
