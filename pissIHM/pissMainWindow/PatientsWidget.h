#ifndef PATIENTSWIDGET_H
#define PATIENTSWIDGET_H

#include <QWidget>
#include <QObject>
#include <QListWidget>
#include <QVBoxLayout>
#include <Patients.h>
#include <QDebug>
#include <QTextEdit>
#include <AlgorithmTestPlatform.h>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTime>
#include <QMouseEvent>
#include <QColor>
#include <QPalette>
#include <QBrush>
#include <QPixmap>
#include <QBitmap>
#include <QProgressDialog>
#include <QSpinBox>
#include <QString>

#include "SystemDispatcher.h"
#include "DicomCDRomReader.h"
#include "SurgeryPlanWindow.h"
#include "GuidewareTrackingWindow.h"
#include "PatientInformationWidget.h"
#include "ControlConsoleWindow.h"
#include "PatientItem.h"
#include "SuperviseWindow.h"
#include "Patient.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "PlottingBoard.h"
#include "igsssCutter.h"
#include "vtkResliceCursorCallback.h"
#include "vtkResliceCursor.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkObject.h"
#include "PatientWidgetConfigurationBoard.h"

#include "vtkOutlineFilter.h"
#include "vtkProperty.h"
#include "vtkPolyDataMapper.h"
#include "vtkAxesActor.h"

//! -------------------------------------------------------------------------
//!
//! \brief The Screen struct
//!
typedef struct{
    int screenIndex;
    QRect rect;
}SScreen;

/**
 * @brief The PatientsWidget class
 */
class PatientsWidget : public QWidget
{
    Q_OBJECT
public:
    PatientsWidget(SystemDispatcher* dispatcher,
                   AlgorithmTestPlatform *algorithmTestPlatform,
                   PatientInformationWidget* patientInformationWidget,
                   QTime* surgeryTimer,
                   QFont *caracterStyle,
                   int appWidth,
                   int appHeight,
                   QString workspaceColor,
                   QString globalFontColor);

    void drawBackground(QWidget *widget, QString path, int w, int h);
    void terminate();

    void initVariable();
    void constructIHM();

    void findPatientExisted();
    void setConnections();
    void addPatientToWidget(QString path);
    void displayLastFiveOrLess();
    void keyPressEvent(QKeyEvent *event);
    void displayCurrentPatinetInfo();
    void visualizeCurrentVolumeData();
    void display(vtkImageData *imgToBeDisplayed);
    void setPatientHandling(Patient *patientHandling);
    void setWorkSpaceColor(QString workspaceColor);

    bool loadMRAImageFile(const QString &fileName);
    void update();

    int testX();
    int testY();

    void ResetViews();
    void commandAnalyse(QString cmd);
    void generatePointAt(double x, double y, double z, double radius);
    void resetConfigurationFile();
    void documenter(QString path);
    void copyPathAndDeleteFiles(QString sourceDir, QString destinationDir);
    QVector<vtkActor*> constructEachBranchByCoordinatesAndRadius(QString fileName);
    vtkActor* visualiserInterventionalChemin(vtkPoints* vReconstructPoints);

private:
    vtkPolyVertex *singleVesselPoly;

    PatientWidgetConfigurationBoard *tissueExtractionConfigurationBoard;
    ImageProcessingFactory *imageProcessingFactory;
    bool isCameraFlyThroughButtonClicked;
    bool isGuidewareMovementButtonClicked;

    bool flyThroughMode;
    int flyThroughCpt;

    CenterLineReader centerLineReader;

    QTimer *flyThroughTimer;

    vtkPoints *vessel;
    int vesselPointCount;

    QVector <vtkActor *> centerlineActorSet;
    vtkActor *cuttingLayerOptionActor;
    vtkVolume *volume;

    vtkFixedPointVolumeRayCastMapper *volumeMapper;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderer> originVolumeDataRenderer;
    vtkVolumeProperty*volumeProperty;

    vtkSmartPointer<vtkRenderWindow> flyThroughRenderWindow;
    vtkSmartPointer<vtkRenderer> flyThroughRenderer;

    vtkImageData* currentVolumeData;
    vtkImageShiftScale *shiftScaleVolumeData;

    vtkCamera *originVolumeDataCamera;
    vtkCamera *flyThroughCamera;

    QString workspaceColor;
    QString globalFontColor;

    int workspaceRed;
    int workspaceGreen;
    int workspaceBlue;

    SystemDispatcher* dispatcher;
    SuperviseWindow *superviseWindow;
    Patient* patientHandling;

    QVBoxLayout* patientsWidgetLayout;
    QGridLayout* patientsWidgetWorkspaceLayout;
    QHBoxLayout* patientsPhotoWidgetLayout;
    QVBoxLayout* waittingPatientsAndBlackAreaWindowLayout;
    QHBoxLayout* waittingPatientPhotoesLayout;
    QVBoxLayout* patientHandlingWidgetLayout;
    QHBoxLayout* patientsWidgetToolBarLayout;
    QGridLayout* patientInfoContainerLayout;
    QVBoxLayout* firstPatientPhotoWidgetLayout;
    QVBoxLayout* secondPatientPhotoWidgetLayout;
    QVBoxLayout* thirdPatientPhotoWidgetLayout;
    QVBoxLayout* fourthPatientPhotoWidgetLayout;

    QWidget* patientsWidgetToolBar;
    QLabel* patientsPhotoWidget;

    QLabel *patientImageDispalyArea;
    QLabel *imageConfigurationArea;

    QPushButton *guidewareMovementButton;
    QPushButton *cameraFlyThroughButton;
    QPushButton *clearCenterLineButton;
    QPushButton *autoVolumeRenderingButton;
    QPushButton *threeSliceEnableButton;
    QPushButton *tissueExtractionButton;
    QLineEdit * vesselModelingLineEdit;

    QComboBox *modalityOptionComboBox;

    QSpacerItem *imageConfigurationAreaSpacer;

    QHBoxLayout *imageConfigurationAreaLayout;

    QVBoxLayout *patientImageDispalyAreaLayout;

    QVTKWidget* currentPatientVolumeDataAnalyseArea;

    QWidget*medicalImageAnalyseArea;
    QHBoxLayout *medicalImageAnalyseAreaLayout;

    QWidget* patientsWidgetWorkspace;

    QWidget* patientHandlingWidget;
    QWidget* waittingPatientPhotoes;
    QWidget* downBlackArea;
    QWidget* leftBlackArea;
    QLabel* patientInfoContainer;

    PatientItem* firstPatientPhotoWidget;
    PatientItem* secondPatientPhotoWidget;
    PatientItem* thirdPatientPhotoWidget;
    PatientItem* fourthPatientPhotoWidget;


    QLabel*currentPatientPhoto;
    QVBoxLayout*currentPatientPhotoLayout;
    QLabel*currentPatientInfo;
    QWidget *patientPhotoBlankArea;

    QLabel* nameLabel;
    QLabel* birthdayLabel;
    QLabel* sexualLabel;
    QLabel* ageLabel;
    QLabel* idNumberLabel;
    QLabel* marryLabel;
    QLabel* nationLabel;
    QLabel* professionalLabel;
    QLabel* drugAllergyLabel;
    QLabel* remarksLabel;
    QLabel* leadDoctorLabel;
    QLabel* therapyTimeLabel;

    QLabel *surgeryPlanArea;
    QVBoxLayout *surgeryPlanAreaLayout;
    QLabel *flyThroughtConfigurationBar;
    QHBoxLayout *flyThroughtConfigurationBarLayout;
    QVTKWidget *interventionalRoutePlanningArea;
    QSpacerItem* flyThroughtConfigurationBarSpacer;

    QPushButton *cutButton;
    QPushButton *reconstructButton;
    QSpinBox *cuttingLayerOption;

    QLabel *analyseResultDisplayArea;
    QVBoxLayout *analyseResultDisplayAreaLayout;

    QLineEdit* nameLineEdit;
    QLineEdit* birthdayLineEdit;
    QLineEdit* sexualLineEdit;
    QLineEdit* ageLineEdit;
    QLineEdit* idNumberEdit;
    QLineEdit* marryLineEdit;
    QLineEdit* nationLineEdit;
    QLineEdit* professionalLineEdit;
    QLineEdit* drugAllergyLineEdit;
    QLineEdit* remarksLineEdit;
    QLineEdit * leadDoctorEdit;
    QLineEdit* therapyTimeEdit;

    QFont *caracterStyle;

    QWidget* waittingPatientsAndBlackAreaWindow;

    QPushButton* leftSelectButton;
    QPushButton* rightSelectButton;
    QPushButton* cdRomParseButton;
    QPushButton* addPatientButton;
    QPushButton* surgeryLaunchButton;
    QPushButton* patientsWidgetConfigurationButton;
    QPushButton* simpleModeButton;

    AlgorithmTestPlatform *algorithmTestPlatform;

    PatientInformationWidget* patientInformationWidget;


    QSpacerItem* controlBarSpacer;
    //QSpacerItem* patientInfoContainerSpacer;

    DicomCDRomReader *dicomCDRomReader;

    QList<QLabel*> waitingPatientsQueue;
    QList<QLabel*> waitingPatientsNameQueue;
    QStringList currentPatinetInfoQueue;

    QStringList waitingPatientsPhotoStringQueue;
    QStringList waitingPatientsStringNameQueue;
    QStringList waitingPatientsIDQueue;
    QStringList waitingPatientsMraPathQueue;

    int appWidth;
    int appHeight;
    int numberOfPatient;
    bool surgeryLaunched;
    bool terminated;

    QString labelStyleSheet;
    QString photoLabelStyleSheet;
    QString textEditStyleSheet;
    QTime* surgeryTimer;

//---------------add 3 view--------------------------
    QVTKWidget * xySlice;
    QVTKWidget * yzSlice;
    QVTKWidget * xzSlice;
    QLabel * slicingConfigurationBar;
    QGridLayout *threeViewAreaLayout;

    vtkSmartPointer< vtkResliceImageViewer > riw[3];

    vtkSmartPointer< vtkDistanceWidget > DistanceWidget[3];
    vtkSmartPointer< vtkResliceImageViewerMeasurements > ResliceMeasurements;
    vtkSmartPointer<vtkCellPicker> picker;
    vtkSmartPointer<vtkProperty> ipwProp;
    vtkSmartPointer< vtkImagePlaneWidget > planeWidget[3];
//---------------add 3 view--------------------------
    bool simpleViewButtonIsClicked;

    QStringList modalities;
    QString currentModality;

    vtkEventQtSlotConnect* currentPatientVolumeDataAnalyseAreaConnections;


    QString configuratonFilePath;
    SScreen *screen;
    QDesktopWidget *desktop;
    int primary_screen_width;
    int primary_screen_height;
    int screen_count;
    int primary_screen;

    SurgeryPlanWindow* surgeryPlanWindow;
    GuidewareTrackingWindow*  guidewareTrackingWindow;
    ControlConsoleWindow * controlConsoleWindow;

signals:
    void surgeryLaunchButtonCicked();
    void surgeryFinished();

public slots:
    void simpleViewButtonClicked();
    void doParseCdRom();
    void launchSurgery();
    //void terminateSurgery();
    void addPatient();
    void doLeftSelect();
    void doRightSelect();
    void onGuidewareMovementButtonClicked();
    void onCameraFlyThroughButtonClicked();
    void onClearCenterLineButtonClicked();
    void autoVolumeRenderingButtonClicked();
    void threeSliceEnableButtonClicked();
    void tissueExtractionButtonClicked();
    void modalityOptionComboBoxChanged(int index);

    void reconstructButtonClicked();
    void vesselModelingLineEditPressed();

    void flyThrough();
    void onCutButtonClicked();
    void cuttingLayerOptionChanged(int value);
    void updateCoords(vtkObject* obj);
    void terminateSurgery(QString path);
};

#endif // PATIENTSWIDGET_H
