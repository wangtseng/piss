#ifndef SURGERYPLANWINDOW_H
#define SURGERYPLANWINDOW_H

#include <QWidget>
#include <QFrame>
#include <QSplitter>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFileInfo>
#include <QDir>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QLCDNumber>
#include <QTime>
#include <QDateTime>
#include <QVector>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QPoint>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QColor>
//ADD MC
#include<vtkMarchingCubes.h>
#include<vtkStripper.h>
#include<vtkPolyDataMapper.h>
#include<vtkPolyDataWriter.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL);


#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkImageViewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <vtkMetaImageReader.h>
#include <vtkImageViewer2.h>

#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <QVTKWidget.h>
#include <vtkImageData.h>
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyVertex.h>
#include <vtkPoints.h>
#include <vtkProperty.h>

#include <vtkOBJReader.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkSplineDrivenImageSlicer.h>
#include <Patient.h>
#include <CPushButton.h>
#include <PlottingBoard.h>
#include <PatientWidgetConfigurationBoard.h>
#include <ColorChooseWindow.h>
#include <vtkEventQtSlotConnect.h>
#include <SystemDispatcher.h>
#include <vtkImageAppend.h>
#include "CurveReformationWindow.h"
#include "AlgorithmTestPlatform.h"
#include "CenterLinePoint.h"
#include "CenterLineReader.h"
#include <vtkSCurveSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include "vtkDataSetMapper.h"
#include <vtkSCurveSpline.h>
#include <vtkParametricSpline.h>
#include <vtkCamera.h>
#include <vtkDoubleArray.h>
#include <vtkTubeFilter.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkProperty.h>
#include <vtkDoubleArray.h>
#include "vtkMetaImageWriter.h"
#include "vtkDecimatePro.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataNormals.h"
#include"vtkPolyDataMapper.h"
#include "vtkImageDataToPointSet.h"
#include "vtkOutlineFilter.h"
#include "vtkSTLReader.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkSTLWriter.h"

struct TransferOptionStates{
    bool opacityTransferOptionChoosen = false;
    bool colorTransferOptionChoosen = false;
    bool gradientTransferOptionChoosen = false;
};

struct TransferCurveIndex{
    int opacityTransferCurveIndex;
    int colorTransferCurveIndex;
    int gradientTransferCurveIndex;
};

struct ImageOptionStates{
    bool originalOptionState;
    bool transparentBrainOptionState;
    bool greyMatterOptionState;
    bool whiteMatterOptionState;
    bool vesselOptionState;
    bool interventionalRouteOptionState;
};


/**
 * @brief The SurgeryPlanWindow class
 */
class SurgeryPlanWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SurgeryPlanWindow(QRect rect,
                               QTime* surgeryTime,
                               SystemDispatcher* systemDispatcher,
                               QString globalWorkSpaceColor,
                               QString configuratonFilePath);
    void displayWindow();

    void createRandomVtkImageData();
    void display(vtkImageData *imgToBeDisplayed);
    void displayBrainandPath(vtkImageData *imgToBeDisplayed,QString referencePath);
    void displaySize();
    void setPatientHandling(Patient *patientHandling);
    void update();
    void loadVesselsExisted();
    void setStartTime(int start_time);

    void setWorkSpaceColor(QString workspaceColor);

    void constructTridimensionalImageVisualisingArea();
    void constructTridimensionalImageAnalysingArea();
    void constructControlBar();
    void constructIHM();

    void initialisation();
    void regroupAllComponents();
    void setConnections();
    void drawBackground();
    void configuration();

    void updatePatientPersonelInformation();
    void updatePatientPersonelPhoto();

    //!   show slice
    void showSlice(int position);

    void displayVessel();
    void displayCpr();
    void displayCenterLine();
    void displayPatientMRAImage();
    void updatePatientMRAImageStatistics();
    void updatePatientMRAImageHistogram();
    void updatePatientMRAImageTransformationCurves();
    void removeCurveBy(int index);
    void updatePatientMRAImage();
    void generateNewOpacityPoint(double abscissa, double ordinate);
    void generateNewColorPoints(double abscissa, double ordinate);
    void generateNewGradientPoint(double abscissa, double ordinate);
    void generateInitColorPoints(double abscissa, int colorCount);
    void generateInitColorPoints(double abscissa, int red, int green, int blue);


    void initialRendering();
    void opacityPointTracking();
    void colorPointTracking();
    void gradientPointTracking();
    //void calculateCenterOfTheVolume();
    void sfunctionSource();
    void cprMath();
    vtkSmartPointer<vtkActor> generatePolyDataActorFromMetaFile(QString path, int r, int g, int b,int iterations);
    vtkSmartPointer<vtkActor> generatePolyDataActorFrom(vtkImageData* input, int r, int g, int b,int iterations);
    vtkSmartPointer<vtkActor> generateBoundBox(vtkImageData*img);
    void displayTragetArchitecture();
    void documenter();
    void reconstructButtonClicked();
    QVector<vtkActor*> constructEachBranchByCoordinatesAndRadius(QVector<CenterLinePoint*>* vesselPoints);
    vtkActor* visualiserInterventionalChemin(vtkPoints* vReconstructPoints);

protected:
    vtkEventQtSlotConnect* MraConnections;

private:
    QStringList ndiInfo;
    int ndiInfoCount;
    vtkActor *previousNDIPosActor;

    vtkSmartPointer<vtkActor> grayMatterActor;
    vtkSmartPointer<vtkActor> whiteMatterActor;
    vtkSmartPointer<vtkActor> vesselBackActor;
    vtkSmartPointer<vtkActor> vesselFrontActor;
    vtkSmartPointer<vtkActor> vesselLeftActor;
    vtkSmartPointer<vtkActor> vesselRightActor;
    QMap<QString, vtkActor*> vesselActors;
    QMap<QString, vtkActor*> cheminActors;

    vtkSmartPointer<vtkActor> vesselActor;

    //! ---------------------------------------------------------------------------------------------------------
    //! MRI
    QVTKWidget *patientMRAImage;

    //! ---------------------------------------------------------------------------------------------------------
    //! Centre line
    QVTKWidget *centerLineVTKWidget;

    vtkPolyVertex *singleVesselPoly;
    vtkUnstructuredGrid *singleVesselgrid;
    vtkDataSetMapper *singleVesselMapper;
    vtkActor *singleVesselactor;
    vtkRenderer *singleVesselRenderer;
    vtkRenderWindow *singleVesselRenderwindow;

    //! ---------------------------------------------------------------------------------------------------------
    //! CPR Results
    QVTKWidget *cprOutcomingVTKWidget;

    //! ---------------------------------------------------------------------------------------------------------
    int workspaceRed;
    int workspaceGreen;
    int workspaceBlue;
    QString vesselHandlingName;
    QRect rect;

    QTime* surgeryTime;

    QTimer *timerToWaittingReconstructedResult;
    QTimer *realTimeTrackingNDITimer;

    double transformationPlottingBoard_AbscissaValue;
    double transformationPlottingBoard_OrdinateValue;
    bool transformationPlottingBoard_Clicked;
    int lockingTransformationPointIndex;

    ColorChooseWindow* colorChooseWindow;

    PatientWidgetConfigurationBoard *patientWidgetConfigurationBoard;

    //MedicalImageHandler *medicalImageHandler;
    QString mraImagefilePath;
    QString globalWorkSpaceColor;
    QFrame *controlBar;
    QHBoxLayout *controlBarLayout;

    CPushButton *imageConfigurationButton;
    CPushButton *imageUpdateButton;

    CPushButton *dislayImageAnalyseAreaButton;
    bool displayImageAnalyseAreaButtonClicked;

    CPushButton *beginTherapyButton;
    QSpacerItem *spacer_controlbar;


    QFrame *patientTitleBar;
    QHBoxLayout *patientTitleBarLayout;
    QWidget *briefWidget;
    QLabel *patientTitleLabel;
    QLabel *photoWidget;
    QSpacerItem *spacer_1;
    QSpacerItem *spacer_2;

    QFrame *patientClinicalWidgetsContainer;
    QHBoxLayout *patientClinicalWidgetsContainerLayout;
    QWidget *patientMRAImageOptionWidget;
    QVBoxLayout *patientMRAImageOptionWidgetLayout;

    QWidget *patientImageInformationWidget;
    QHBoxLayout* patientImageInformationWidgetLayout;
    QLabel* surgeryPlanWindowLabel;
    QLCDNumber *elapsedTimeLabel;
    //QPushButton* sugeryEndnessButton;

    QGroupBox * histogramGroupBox;
    QVBoxLayout *histogramGroupBoxLayout;
    QWidget *imageStatisticsWidget;
    QGridLayout *imageStatisticsWidgetLayout;

    QLabel *grayscaleValueNumberLabel;
    QLabel *grayscaleValueMeanLabel;
    QLabel *grayscaleValueMedianLabel;
    QLabel *grayscaleValueStandardDeviationLabel;
    QLabel *grayscaleValueMaximumValueLabel;
    QLabel *grayscaleValueMinimumValueLabel;

    QLineEdit *grayscaleValueNumberLineEdit;
    QLineEdit *grayscaleValueMeanLineEdit;
    QLineEdit *grayscaleValueMedianLineEdit;
    QLineEdit *grayscaleValueStandardDeviationLineEdit;
    QLineEdit *grayscaleValueMaximumValueLineEdit;
    QLineEdit *grayscaleValueMinimumValueLineEdit;

    PlottingBoard *histogramPlottingBoard;

    QGroupBox * volumeRenderingGroupBox;
    QVBoxLayout *volumeRenderingGroupBoxLayout;
    QWidget *transferChoicesWidget;
    QHBoxLayout *transferChoicesLayout;
    QLabel *transferChoiceLabel;
    QRadioButton *opacityTransferChoice;
    QRadioButton *colorTransferChoice;
    QRadioButton *gradientTransferChoice;
    QSpacerItem *transferChoicesWidgetSpacer;

    PlottingBoard *transformationPlottingBoard;

    QWidget *transformationIndicationBar;
    QHBoxLayout *transformationIndicationBarLayout;
    QLabel *currentGrayScaleValueLabel;
    QLabel *currentTransformationValueLabel;
    CPushButton *transformationButton;
    QLineEdit *currentGrayScaleValueLineEdit;
    QLineEdit *currentTransformationValueLineEdit;


    QSpacerItem *patientMRAImageOptionWidgetSpacer;

    QWidget *patientMRAImageConfigurationWidget;
    QVBoxLayout *patientMRAImageConfigurationWidgetLayout;
    QWidget *patientMRAImageOption;
    QHBoxLayout *patientMRAImageOptionLayout;

    CPushButton *originalOption;
    CPushButton *transparentBrainOption;
    CPushButton *greyMatterOption;
    CPushButton *whiteMatterOption;
    CPushButton *vesselOption;
    CPushButton *interventionalRouteOption;
    QPushButton *quitSurgeryPlanButton;

    ImageOptionStates imageOptionStates;

    QLabel *patientMRAImageOptionLabel;

    QWidget *patientMRAImageWidget;
    QHBoxLayout *patientMRAImageWidgetLayout;


    QWidget *patientMRAImageManipulation;


    vtkActor *actorreferencePath;//...
    vtkDataSetMapper *mapperreferencePath;
    vtkUnstructuredGrid *grid;
    vtkPolyVertex *poly;
    vtkPoints *points;

    vtkFixedPointVolumeRayCastMapper *volumeMapper;

    //ADD MC
//    vtkSmartPointer<vtkMetaImageReader> reader;
//    vtkSmartPointer<vtkMarchingCubes> MC;
//    vtkSmartPointer<vtkDecimatePro> deci;
//    vtkSmartPointer<vtkSmoothPolyDataFilter> smooth;
//    vtkSmartPointer<vtkPolyDataNormals> normals;
//    vtkSmartPointer<vtkStripper> stripperfilter;
//    vtkSmartPointer<vtkPolyDataMapper> MC_mapper;

//    vtkSmartPointer<vtkActor> MC_actor;

    vtkVolume *volume;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderer> mainRenderer;

    vtkPiecewiseFunction*opacityTransferFunction;
    vtkPiecewiseFunction *gradientTransferFunction;
    vtkColorTransferFunction *colorTransferFunction;
    vtkVolumeProperty*volumeProperty;

    //vtkRenderWindowInteractor * iren;
    vtkInteractorStyleTrackballCamera *style;
    //vtkMyCallback *callBack;

    TransferOptionStates transferOptionStates;
    TransferCurveIndex transferCurveIndex;

    // cpr information widget
    QFrame *imageAnalyseArea;
    QHBoxLayout *cprAnalyseWidgetLayout;
    QTreeWidget *centerlineTreeWidget;
    QTreeWidgetItem *vesselsFolder;
    QIcon *defaultFolderIcon;
    QIcon *fileUnloadedIcon;
    QIcon *fileLoadedIcon;
    QIcon *defaultTitleIcon;
    QFont *font;

    QWidget *centrelineVisualiseArea;
    QVBoxLayout *centrelineVisualiseAreaLayout;

    QLabel *centrelineParametorArea;
    QHBoxLayout *centrelineParametorAreaLayout;
    QSlider *centrelineSlider;
    QComboBox *centreLineCoordinates;

    vtkPoints *singleVesselPoints;

    vtkPolyDataMapper *cprmapper;
    vtkFixedPointVolumeRayCastMapper *cprmapper1;
    vtkActor *cpractor;
    vtkSCurveSpline *xSpline;
    vtkSCurveSpline *ySpline;
    vtkSCurveSpline *zSpline;
    vtkParametricSpline *spline;
    vtkParametricFunctionSource *functionSource;
    vtkSplineDrivenImageSlicer *reslicer;
    vtkImageAppend *append;


    vtkImageShiftScale *m_pShift;
    vtkPlane *pPlane;
    vtkCutter *pCut;

    vtkColorTransferFunction *cprcolorTranFun;
    vtkPiecewiseFunction *cprPieceFun;
    vtkVolumeProperty *cprVolumeproperty;
    vtkVolume *cprvolume;
    vtkRenderer *cprRenderer;
    vtkRenderWindow *cprRenderwindow;


    QWidget *flyThroughWidget;
    QVBoxLayout *mainLayout;
    Patient *patientHandling;

    vtkImageData *enhancedImage = NULL;

    int x;
    int y;
    int appWidth;
    int appHeight;
    SystemDispatcher* systemDispatcher;
    int start_time;
    int current_time;

    //! in doVesselReconstruct()
    vtkPoints* vReconstructPoints;
    vtkCellArray* vReconstructLines;
    vtkPolyData* vReconstructPolyData;
    vtkDoubleArray* vReconstructRadius;
    vtkTubeFilter* vReconstructTube;
    vtkPolyDataMapper* vReconstructMapper;
    vtkActor* vReconstructActor;

    QString configuratonFilePath;

    vtkOrientationMarkerWidget* axes;

    QVector<igtNDIPosition *> positionList;

signals:
    void finished();
    void error(QString err);

public slots:
    void process();
    void opacityTransformationStateChanged(bool state);
    void colorTransformationStateChanged(bool state);
    void gradientTransformationStateChanged(bool state);
    void generateNewTransformationPoint(QMouseEvent* event);
    void transformPointTracking(QMouseEvent* event);
    void lockTargetPoint(QMouseEvent* event);
    void positioningTransformPoint(QMouseEvent* event);
    void updateCoords(vtkObject* obj);

    void originalOptionHovered();
    void originalOptionClicked();
    void originalOptionReleased();
    void originalOptionLeaved();

    void transparentBrainOptionHovered();
    void transparentBrainOptionClicked();
    void transparentBrainOptionReleased();
    void transparentBrainOptionLeaved();

    void greyMatterOptionHovered();
    void greyMatterOptionClicked();
    void greyMatterOptionReleased();
    void greyMatterOptionLeaved();

    void whiteMatterOptionHovered();
    void whiteMatterOptionClicked();
    void whiteMatterOptionReleased();
    void whiteMatterOptionLeaved();

    void vesselOptionHovered();
    void vesselOptionClicked();
    void vesselOptionReleased();
    void vesselOptionLeaved();

    void interventionalRouteOptionHovered();
    void interventionalRouteOptionClicked();
    void interventionalRouteOptionReleased();
    void interventionalRouteOptionLeaved();

    void displayConfigurationBoard();
    void getVesselEnhancedImage();
    void stopSurgery();

    void displayImageAnalyseArea();
    void closeSurgeryPlanWindow();

    void toFindReconstructedResult();
    void showContextMenu(const QPoint &pos);
    void loadVesselAction();
    void unloadVesselAction();
    void deleteVesselAction();

    void doVesselReconstruct();
    void updateNDIPos();


signals:



};

#endif // SURGERYPLANWINDOW_H
