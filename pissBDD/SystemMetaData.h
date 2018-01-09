#ifndef SYSTEMMETADATA_H
#define SYSTEMMETADATA_H

#include <QString>


class SystemMetaData
{
public:
    SystemMetaData();
    void setSystemPaths(QString workspace_path,  QString patients_path, QString metadata_path, QString image_center_path);
    QString getSystemWorkSpace();
    QString getPatientsPath();
    QString getMetaData();
    QString getImageCenterPath();

private:
    QString workspace_path;
    QString patients_path;
    QString metadata_path;
    QString image_center_path;
};

#endif // SYSTEMMETADATA_H
