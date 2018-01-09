#ifndef CENTERLINEREADER_H
#define CENTERLINEREADER_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QString>
#include <QChar>
#include <QDebug>
#include <QByteArray>
#include <QVector>
#include "CenterLinePoint.h"
#include "vtkPoints.h"

class CenterLineReader
{
public:
    CenterLineReader();
    ~CenterLineReader();


    QVector<CenterLinePoint*>  doReadComplteCenterlineFile(QString centerlineFilePath);
    int doReadCenterLineFile(QString centerlineFilePath, vtkPoints *vessel);
    void doReadCenterlineFolder(QString centerlineFolderPath);
    QVector<CenterLinePoint*> get_vesselsPoints();

private:
    QVector<CenterLinePoint*> vesselsPoints;
};

#endif // CENTERLINEREADER_H
