/*! \file       ImageFileInterface_RawMhd.h
 *  \brief
 *
 *  ................ Description
 *
 *  \author   Cheng WANG
 *  \date     2015.04.29
 *  \version  1.0
****************************************************************************************************************************/

#ifndef IMAGEFILEINTERFACE_RAWMHD_H
#define IMAGEFILEINTERFACE_RAWMHD_H

#include <ImageFileInterface.h>
#include <vtkSmartPointer.h>
#include <vtkMetaImageReader.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkInformation.h>

#include <QString>
#include <QDebug>


class ImageFileInterface_RawMhd : public ImageFileInterface
{
public:
    ImageFileInterface_RawMhd();
    ~ImageFileInterface_RawMhd();

    //! Virtual methods inherited from the class ImageReaderInterface
    /*virtual*/ eImageErrorCode readFrom(const QString &filepath);
    /*virtual*/ vtkImageData* getImage();
    /*virtual*/ void getImageExtentInformation(int* extent);
private:
    vtkImageData *image;
    vtkSmartPointer<vtkMetaImageReader> reader;

};

#endif // IMAGEFILEINTERFACE_RAWMHD_H
