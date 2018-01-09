#ifndef MATFILEREADER_H
#define MATFILEREADER_H

#include <QString>
#include <QStringList>
#include <BidimensionnelImage.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkMetaImageReader.h>
#include <QFile>

class MatFileReader
{
public:
    MatFileReader();
    ~MatFileReader();

    bool doParseMatImageToBiDimensionImage(const QString &MatFilePath, vtkImageData*image);
    bool getReadCompleteFlag();
    void setReadCompleteFlag(bool flag);

private:
    BidimensionnelImage *image;
    bool readCompleteFlag;
    vtkSmartPointer<vtkMetaImageReader> reader;
};

#endif // MATFILEREADER_H
