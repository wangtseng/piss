#ifndef CURVEREFORMATIONWINDOW_H
#define CURVEREFORMATIONWINDOW_H


#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVTKWidget.h>
#include <QPixmap>
#include <QBitmap>
#include <QPalette>
#include <qtreewidget.h>
#include <QTreeWidgetItem>

#include <QWidget>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include "vtkRenderWindowInteractor.h"
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkImageMapToColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkCutter.h"
#include "vtkPlane.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageCast.h"
#include "vtkPointData.h"
#include "vtkImageShiftScale.h"
#include "vtkPolyDataToImageStencil.h"
#include "vtkImageStencil.h"
#include "vtkLookupTable.h"
#include "vtkImagePlaneWidget.h"
#include "vtkCellArray.h"
#include "vtkPlaneWidget.h"
#include "vtkProperty.h"
#include "vtkCommand.h"
#include "vtkProbeFilter.h"
#include "vtkImplicitPlaneWidget.h"
#include "vtkClipPolyData.h"
#include "vtkSphereSource.h"


#include <QFile>
#include <QDebug>
#include "vtkPoints.h"
#include <vector>
#include <QByteArray>

#include <vtkPolyVertex.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>

#include <vtkProperty.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSimplePointsReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkMetaImageReader.h>
#include <vtkImageViewer2.h>

#include <vtkTransform.h>

#include "Patient.h"


class CurveReformationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CurveReformationWindow(int x, int y);
    ~CurveReformationWindow();


    void setPatientHandling(Patient *patientHandling);

    void drawBackground();

    void initVariable();
    void constructIHM();
    void setConnections();

    void display();
signals:

public slots:
    void commence();
    void reset();
    void ferme();

private:

    int width;
    int height;
    Patient *patientHandling;

    QPixmap *pixmap;
    QGridLayout *myLayout;
    QWidget *referenceTableWidget;
    QVBoxLayout *referenceTableLayout;
    QWidget *curvereformationWidget;
    QLabel *curvereformationLabel;
    QTreeWidget *centerlinetreeTreeWidget;

    QWidget *manipulationWidget;
    QHBoxLayout *manipulationLayout;
    QWidget *manipulationItemWidget;
    QVBoxLayout *manipulationItemLayout;
    QWidget *manipulationindeterminateWidget;
    QPushButton *fermePushBtn;
    QPushButton *resetPushBtn;
    QPushButton *commencePushBtn;


    QWidget *volumeWidget;
    QVTKWidget *volumeVTKWidget;
    QVBoxLayout *volumeWidgetLayout;

    QWidget *centerLineWidget;
    QVTKWidget *centerLineVTKWidget;
    QVBoxLayout *centerLineWidgetLayout;

    QWidget *volumeAndCenterLineMergeWidget;
    QVTKWidget *volumeAndCenterLineMergeVTKWidget;
    QVBoxLayout *volumeAndCenterLineMergeWidgetLayout;

    QWidget *reformatedWindow;
    QVTKWidget *reconstructionVolumeVTKWidget;
    QVTKWidget *cprvtkWidget;
    QHBoxLayout *reformatedWindowLayout;
};
#endif // CURVEREFORMATIONWINDOW_H
