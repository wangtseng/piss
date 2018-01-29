#ifndef PATIENT_H
#define PATIENT_H
#include <QList>
#include <IgssImage.h>
#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <QImage>
#include <QString>
#include <QColor>
#include <QMap>
#include <QVector>
#include <QStringList>
#include "MRAFileReader.h"
#include "vtkImageData.h"
#include "vtkPoints.h"
#include <vtkSmartPointer.h>
#include "IgssVtkImageConverter.h"
#include "ImageProcessingFactory.h"
#include "ImageFileInterface.h"
#include <HistogramPoint.h>
#include <ColorPoint.h>
#include <TransferPoint.h>
#include "BidimensionnelImage.h"
#include "CenterLineReader.h"
#include "CenterLinePoint.h"
#include <QVector>
#include <QMutex>
#include <QDebug>
#include "igtForceTorqueValues.h"
#include "igtMotorParameter.h"
#include "igtAdvancementVelocityIs.h"
#include "OmegaPosition.h"
#include "igtNDIPosition.h"
#include "igtContrastMediaPush.h"
#include "CloseSessionMessage.h"


class Patient
{
private:
    class ThreadImageProcessing : public QThread{
        public:
            void run();
            Patient *patient;
    };

public:
    Patient(QString myPath, int id);
    void configurerEchantionnageEtat(bool echantionnage);
    void setTargetObjectHandling(QString obj);
    QString getTargetObjectHandling();

    bool doReadReconstructedResult();

    int getIdNumber();
    bool readFinished();
    QString getBirthdayOfPatient();
    void doImageFileLecture();
    QString getCTImagePath();
    QString getMagneticTackingDataPath();
    QString getName();

    QString getFirstName();
    QString getLastName();

    QString getSex();
    QString getPhotoPath();
    QString getMraTridimensionelPath();
    QString getTridimensionelPath();
    IgssImage *getOriginImage();
    vtkImageData *getMraImageToBeDisplayed();
    void setImageProcessingFactory(ImageProcessingFactory* imageProcessingFactory);
    bool loadMRAImageFile(const QString &fileName);

    bool loadCenterLineFiles(QString fileName);

    void setMRAImage(vtkImageData *personnelMRAImage);

    QStringList getMriStatisticsList();
    QVector<HistogramPoint*> getMriHistogramfrequencies();
    QVector<HistogramPoint*> getGrayScaleValuefrequencies();
    QVector<TransferPoint*> getOpacityTransferPoints();
    QVector<ColorPoint*> getColorTransferPoints();
    QVector<TransferPoint*> getGradientTransferPoints();

    int getColorTransferPointsNumber();

    int findPointInTolerentArea(double abscissa, double ordinate, QString transformationFormat);

    void setGrayScaleValueFrequencies(QVector<HistogramPoint*> frequencies);
    void setOpacityTransferPoint(TransferPoint* opacityPoint);
    void setColorTransferPoint(ColorPoint* redPoint);
    void setGradientTransferPoint(TransferPoint* gradientPoint);

    int getMaximumGrayscaleValue();
    int getMinimumGrayscaleValue();

    int getGrayScaleValueByIndex(int index, QString option);
    double getOpacityValueByIndex(int index);
    double getGradientValueByIndex(int index);
    double getRedValueByIndex(int index);
    double getGreenValueByIndex(int index);
    double getBlueValueByIndex(int index);

    void updateOpacityPoint(int index, int abscissa, double ordinate);
    void updateGradientPoint(int index, int abscissa, double ordinate);
    void updateColorPoint(int index, int abscissa);

    void pushCTImage(BidimensionnelImage *img);
    QImage fetchLatestCTImageForDisplay();
    int getCTImageSequenceLength();

    QImage vtkImageDataToQImage(vtkImageData* imageData);

    vtkPoints *getCenterLinePoints();//! to be deleted

    vtkPoints*getCenterlineByName(QString name);

    QVector<CenterLinePoint*> getVesselByName(QString name);

    int getCenterLinePointsCount();

    QString getCenterLineFolderPath();
    void loadVesselByPath(QString filename, QString path);

    //! -----------------omega bdd-------------------------------------------------------------
    void appendProgressCommand(int currentRpmValue, int currentSymbol, int currentMotorType);
    void appendRotateCommand(int currentRpmValue, bool currentSymbol, int currentMotorType);
    void appendMovingCommand(int speed, int valeur, int unit);
    void appendInjectionCommand(double volume, double speed);
    void appendCloseSessionCommand();

    int getProgressCommandNumber();
    int getRotateCommandNumber();
    int getInjectionCommandNumber();
    int getCloseSessionCommandNumber();

    bool progressCommandsSequenceIsEmpty();
    bool rotateCommandsSequenceIsEmpty();
    bool injectionCommandsSequenceIsEmpty();
    bool closeSessionCommandSequenceIsEmpty();

    void clearAllBuffers();

    igtMotorParameter *fetchProgressMotorParameter();
    void removeFirstProgressMotorParameter();

    igtMotorParameter *fetchRotateMotorParameter();
    void removeFirstRotateMotorParameter();

    igtContrastMediaPush *fetchInjectionCommand();
    void removeFirstInjectionCommand();

    igtCloseSessionMessage *fetchCloseSessionMessage();
    void removeFirstCloseSessionMessage();

    void appendAdvancementTargetVelocity(igtAdvancementTargetVelocity* advancementTargetVelocity);
    int getAdvancementTargetVelocityNumber();
    bool AdvancementTargetVelocitySequenceIsEmpty();
    igtAdvancementTargetVelocity *fetchAdvancementTargetVelocity();
    void removeFirstAdvancementTargetVelocity();

    void setOmegaPosition(double px, double py, double pz);

    bool getLatestProgressSpeed(double *speed);

    void appendForceTorqueValues(igtForceTorqueValues* forceTorqueValues);
    int getForceTorqueNumber();
    bool forceTorqueSequenceIsEmpty();
    igtForceTorqueValues *fetchForceTorqueValues();
    void removeFirstForceTorqueValues();

    void appendNDIPosition(igtNDIPosition* NDIPosition);
    int getNDIPositionSequenceNumber();
    bool NDIPositionSequenceIsEmpty();
    igtNDIPosition *fetchNDIPosition();
    void removeFirstNDIPosition();

    double fetchGuidewireMovingDistance();
    void removeGuidewireMovingDistance();

    //!---------------------------------------------------------------------------------------------------------
    //!
    //! \brief Patient::appendNDIPosition
    //! \param NDIPosition
    //!
    void appendGuidewireMovingDistanceSequence(int NDIPosition);

    //! ----------------------------------------------------------------------------------------
    //!
    //! \brief Patient::getNDIPositionSequenceNumber
    //! \return
    //!
    int getGuidewireMovingDistanceSequenceNumber();
    //! ----------------------------------------------------------------------------------------
    //!
    //! \brief Patient::NDIPositionSequenceIsEmpty
    //! \return
    //!
    bool GuidewireMovingDistanceSequenceIsEmpty();

    QList<QString> getCenterLines();


private:
    bool echantionnage;
    QString controlTarget;
    CenterLineReader *centerReader;

    vtkPoints *centerLinePoints;//! to be deleted
    QMap<QString, vtkPoints*> vesselCenterlines;//! to be deleted

    QMap<QString, QString> vesselNames;
    QMap<QString, QVector<CenterLinePoint*>> vessels;
    QMap<QString, vtkActor*>vesselActors;

    int centerLinePointsCount;

    QVector<HistogramPoint*> frequencies;

    QVector<TransferPoint*> opacityTransferPoints;
    QVector<ColorPoint*> colorTransferPoints;
    QVector<TransferPoint*> gradientTransferPoints;

    QStringList statisticsList;

    int maximumGrayScaleValue;
    int minimumGrayScaleValue;

    bool MraImageReadComplete;

    QString myPath;
    QString myPhotoPath;
    QString mraTridimensionelPath;
    QString centerLineFolderPath;

    //! personnel informations
    QString name;
    QString birthday;
    QString firstName;
    QString lastName;
    QString sex;
    QString IdentityNumber;
    //QString sugeryDate;
    int imageExtents[6];

    int age;
    int id;

    ImageProcessingFactory* imageProcessingFactory;

    //! tridimensionnel Image (MRA)
    IgssImage *originImage;
    vtkImageData* MriImageForDisplay;

    //! bidimensionnel Image (CT) captured while operation
    QList<BidimensionnelImage *> cImageSequence;
    QList<QImage> cImageSequenceForDisplay;

    //! parameter of image......

    //! Handle du thread
    ThreadImageProcessing imageProcessingThread;
    MRAFileReader *mhdFileReader;

    //! -----------------omega bdd-------------------------------------------------------------
    //! input
    QVector<igtAdvancementTargetVelocity*> advancementTargetVelocitySequence;
    QMutex AdvancementTargetVelocityMutex;

    QVector<igtForceTorqueValues*> forceTorqueValuesSequence;
    QMutex ForceTorqueValuesMutex;

    QVector<igtNDIPosition *> NDIPositionSequence;
    QMutex NDIPositionMutex;

    QVector<double> guidewireMovingDistanceSequence;
    QMutex guidewireMovingDistanceMutex;

    //! output
    QVector<igtMotorParameter*> progressCommandsSequence;
    QMutex progressMutex;

    QVector<igtMotorParameter*> rotateCommandsSequence;
    QMutex rotateMutex;

    QVector<igtContrastMediaPush*> injectionCommandsSequence;
    QMutex injectionMutex;

    QVector<igtCloseSessionMessage *> closeSessionCommandsSequence;
    QMutex closeSessionMutex;

    QVector<OmegaPosition *> omegaPositionSequence;
    QMutex omegaPositionMutex;


};

#endif // PATIENT_H
