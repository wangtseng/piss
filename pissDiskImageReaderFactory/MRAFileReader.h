#ifndef MRAFILEREADER_H
#define MRAFILEREADER_H

#include <QString>
#include <QStringList>
#include <IgssImage.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkMetaImageReader.h>
#include <QFile>

class MRAFileReader
{
public:
    MRAFileReader();
    bool doParseMHDFileToIgssImage(QString mhdFilePath, IgssImage*image);
    bool doParseMHDFileToVtkImage(const QString &mhdFilePath, vtkImageData*image);
    bool getReadCompleteFlag();
    void setReadCompleteFlag(bool flag);

private:
    IgssImage *image;
    bool readCompleteFlag;
    vtkSmartPointer<vtkMetaImageReader> reader;
};

#endif // MRAFILEREADER_H
