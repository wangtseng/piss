#ifndef SYSTEMDISPATCHER_H
#define SYSTEMDISPATCHER_H

#include <pissBaseDeDonnee.h>
#include <QProcess>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextEdit>
#include "ImageProcessingFactory.h"
#include "ErrorMessage.h"
#include "igtCommunicationStack.h"
#include "Omega3.h"

/**
 * @brief The SystemDispatcher class
 */
class SystemDispatcher:public QObject
{
    Q_OBJECT
public:
    SystemDispatcher();

    Patient* getPatientById(int id);

    QString get_username();
    QString getImageCenterPath();
    QStringList getPatientsStatus();
    QString doImageProcessingByMethodType(int id, int dim, QString method);

    bool extractBrainCortextFrom(vtkImageData *input, QString type, QString path);

    QVector<HistogramPoint*> getHistogramOfVolumeData(vtkImageData *img);
    QStringList extractHistogramStatistics(vtkImageData *img);

    //! com
    bool launchCommunicationStackServer();
    void launchConnectBackProcess(bool flag, QString addr,int port);
    bool stopPissServer();
    void setCommunicationStack(igtCommunicationStack *communicationStack);
    void setSystemDataBase(pissBaseDeDonnee* database);
    void setImageProcessingFactory(ImageProcessingFactory *imageProcessingFactory);
    void setMessageDisplayWindow(QTextEdit *msgDisplay);
    void setOmega3(Omega3 *omega);
    void initializeSystemPaths();

    void launchOmega(Patient*patientHandling);
    void terminateOmega();

    int findPatientExisted();

    bool lauchServer();
    bool stopServer();
    bool connectera(QString targetIpLineEdit,QString targetPortLineEdit, bool MB);
    int getIncomingConnectionCount();

    QStringList getHistogramStatisticsFrom(vtkImageData* mraImage);

    void setPatientHandling(int patientHandlingID);

private:
    pissBaseDeDonnee* database;
    ImageProcessingFactory *imageProcessingFactory;
    igtCommunicationStack *communicationStack;

    //! -------------create a workspace folder with the default path----------------------------------------
    QString workspace_path;
    QString patients_path;
    QString metadata_path;
    QString image_center_path;

    IgssImage *output;
    Omega3 *omega;

public slots:
    void getSelfIp(QString IP);
    void connectBack(QString remoteIP);

signals:
    void localIPDetect(QString IP);
    void newConnection(QString remoteIP);

};

#endif // SYSTEMDISPATCHER_H
