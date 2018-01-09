#include "DicomCDRomReader.h"


/**
 * @brief DicomCDRomReader::DicomCDRomReader
 */
DicomCDRomReader::DicomCDRomReader(){

}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief DicomCDRomReader::doParseCDRom
//! \param dicomDirPath
//!
void DicomCDRomReader::doParseCDRom(QString dicomDirPath){
    QDir dir(dicomDirPath);

    QFileInfoList files = dir.entryInfoList();
    foreach (QFileInfo file, files){
        if (file.isDir()){
            this->algorithmTestPlatform->setSystemStatus("DIR: "+ file.fileName());
        }else{
            this->algorithmTestPlatform->setSystemStatus("FILE: "+ file.fileName());
        }
    }
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief DicomCDRomReader::setAlgorithmTestPlatform
//! \param algorithmTestPlatform
//!
void DicomCDRomReader::setAlgorithmTestPlatform(AlgorithmTestPlatform *algorithmTestPlatform){
    this->algorithmTestPlatform = algorithmTestPlatform;
}
