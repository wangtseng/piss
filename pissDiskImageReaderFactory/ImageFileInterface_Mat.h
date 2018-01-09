#ifndef IMAGEFILEINTERFACE_MAT_H
#define IMAGEFILEINTERFACE_MAT_H

#include <ImageFileInterface.h>
#include <vtkSmartPointer.h>
#include <vtkMetaImageReader.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkInformation.h>

#include <QString>


class ImageFileInterface_Mat : public ImageFileInterface
{
public:
    ImageFileInterface_Mat();
    ~ImageFileInterface_Mat();


    //! Virtual methods inherited from the class ImageReaderInterface
    /*virtual*/ eImageErrorCode readFrom(const QString &filepath);
    /*virtual*/ vtkImageData* getImage();
    /*virtual*/ void getImageExtentInformation(int* extent);
private:
    vtkImageData *image;
    vtkSmartPointer<vtkMetaImageReader> reader;
};

#endif // IMAGEFILEINTERFACE_MAT_H
