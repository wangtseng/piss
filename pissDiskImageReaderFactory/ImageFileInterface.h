/*! \file       ImageFileInterface.h
 *  \brief
 *
 *  ................ Description
 *
 *  \author   Cheng WANG
 *  \date     2015.04.29
 *  \version  1.0
****************************************************************************************************************************/

#ifndef IMAGEFILEINTERFACE_H
#define IMAGEFILEINTERFACE_H

#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkMetaImageReader.h>

#include <QString>


//! error code to indicate the cause of the error
typedef enum{
    IMAGE_NO_ERRROR = 0,
    IMAGE_MetaFile_ERROR,
    IMAGE_File_ERROR
}eImageErrorCode;

typedef enum {
    UNKOWN_FILE_TYPE = 0,
    RAW_MHD_FILE,
    MHA_FILE,
    DICOMDIR_FILE,
    MAT_FILE
}eFileType;

class ImageFileInterface
{
public:
    //! Constructor of the class ImageReaderInterface
    ImageFileInterface(){}

    //! Destructor of the class ImageReaderInterface
    ~ImageFileInterface(){}

    //! fetch the image object from the file path given
    /*!
     * \brief readFrom
     * \param filepath
     * \param img
     * \return
     */
    virtual eImageErrorCode readFrom(const QString &filepath) = 0;

    virtual void getImageExtentInformation(int* extent) = 0;

    virtual vtkImageData* getImage() = 0;

    static eFileType getFileType(const QString &filepath);

    static ImageFileInterface* getInstanceFileByType(eFileType fileType);

};

#endif // IMAGEREADERINTERFACE_H
