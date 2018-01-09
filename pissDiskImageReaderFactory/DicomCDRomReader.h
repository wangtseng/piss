#ifndef DICOMCDROMREADER_H
#define DICOMCDROMREADER_H

#include <QString>
#include <QDir>
#include <QDebug>

#include "AlgorithmTestPlatform.h"


/**
 * @brief The DicomCDRomReader class
 */
class DicomCDRomReader
{

private:
    AlgorithmTestPlatform *algorithmTestPlatform;

public:
    void setAlgorithmTestPlatform(AlgorithmTestPlatform *algorithmTestPlatform);
    void doParseCDRom(QString dicomDirPath);

public:
    DicomCDRomReader();

};

#endif // DICOMCDROMREADER_H
