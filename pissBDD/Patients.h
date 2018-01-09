#ifndef PATIENTS_H
#define PATIENTS_H

#include <Patient.h>
#include <QList>
#include <QDir>
#include <QDebug>
#include "ImageProcessingFactory.h"


class Patients
{
public:
    Patients();
    int findPatientExisted();
    void setPatientsPath(QString patients_path);
    Patient* getPatientById(int id);
    QStringList getPatientsStatus();
    IgssImage* getPatientMRAImageById(int id);
    QList<Patient*> getPatientsList();
    void setImageProcessingFactory(ImageProcessingFactory* imageProcessingFactory);

private:
    QList<Patient*> patients;
    QString patients_path;
    int id;
    ImageProcessingFactory* imageProcessingFactory;
};

#endif // PATIENTS_H
