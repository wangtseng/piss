#ifndef BASEDEDONNEEPISS_H
#define BASEDEDONNEEPISS_H

#include "Patients.h"
#include "SystemMetaData.h"
#include "ImageProcessingFactory.h"
#include "CArm.h"

#include <QVector>
#include <QMutex>
#include <QDebug>


/**
 * @brief The BASEDEDONNEEPISS class
 *
 *
 *
 */
class pissBaseDeDonnee:public QObject
{
    Q_OBJECT
public:
    pissBaseDeDonnee();

    void setPaths(QString workspace_path,  QString patients_path, QString metadata_path, QString image_center_path);

    QString getSystemWorkSpace();
    QString getPatientsPath();
    QString getMetaData();
    QString getImageCenterPath();

    int findPatientsExisted();
    Patients* getPatientsData();
    Patient* getPatientHandling();
    Patient* getPatientById(int id);
    QStringList getPatientsStatus();

    IgssImage* getPatientMRAImageById(int id);
    void setImageProcessingFactory(ImageProcessingFactory* imageProcessingFactory);
    void setPatientHandling(int patientHandlingID);

public slots:
    void updateDevices();

private:
    int patientHandlingID;
    Patient* patientHandling;
    Patients* patients;
    SystemMetaData* metaData;
    CArm* carm;
    ImageProcessingFactory* imageProcessingFactory;

};

#endif // BASEDEDONNEEPISS_H
