#include "ImageFileInterface.h"

#include <ImageFileInterface_RawMhd.h>
#include <ImageFileInterface_Dicomdir.h>
#include <ImageFileInterface_Mat.h>
#include <QDebug>
#include <QStringList>

eFileType ImageFileInterface::getFileType(const QString &filepath){

    eFileType ret = UNKOWN_FILE_TYPE;

    if(filepath.contains("mhd")){
        ret = RAW_MHD_FILE;
    }
    else if (filepath.contains("mha")) {
        ret = MHA_FILE;
    }
    else if(filepath.contains("DICOMDIR")){
        ret = DICOMDIR_FILE;
    }
    else if(filepath.contains("MAT")){
        ret = MAT_FILE;
    }
    return(ret);
}

ImageFileInterface* ImageFileInterface::getInstanceFileByType(eFileType fileType){

    ImageFileInterface *interface = NULL;

    switch(fileType){
        case RAW_MHD_FILE :
            interface = new ImageFileInterface_RawMhd();
            break;
        case MHA_FILE :
            interface = new ImageFileInterface_RawMhd();
            break;
        case DICOMDIR_FILE:
            interface = new ImageFileInterface_Dicomdir();
            break;
        case MAT_FILE:
            interface = new ImageFileInterface_Mat();
            break;
        default :
            break;
    }

    return(interface);
}


