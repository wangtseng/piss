#ifndef PATIENTWIDGETCONFIGURATIONBOARD_H
#define PATIENTWIDGETCONFIGURATIONBOARD_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QBrush>
#include <QBitmap>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>

#include  <CPushButton.h>
#include  <PlottingBoard.h>
#include  <Patient.h>
//#include <MedicalImageHandler.h>


struct SegmentationButtonStates{
    bool thresholdSegementationState = false;
    bool adaptiveRegionGrowingSegementationState = false;
    bool boostSegementationState = false;
    bool cropVesselSegementationState = false;
};

class PatientWidgetConfigurationBoard : public QWidget
{
    Q_OBJECT
public:
    explicit PatientWidgetConfigurationBoard();
    ~PatientWidgetConfigurationBoard();
     void display(QPoint pos);
     void setPatientHandling(Patient *_patient);

private:
    QHBoxLayout *PatientWidgetConfigurationBoardLayout;

    QWidget *SegmentationOptionWidget;
    QVBoxLayout *SegmentationOptionWidgetLayout;

    QWidget * SegmentationObjectWidget;
    QHBoxLayout *SegmentationObjectWidgetLayout;
    QLabel *SegmentationObjectLabel;
    QComboBox *segmentationObjectComboBox;
    QSpacerItem *segmentationObjectComboBoxSpacer;
    QWidget *SegmentationMethodeWidget;
    QHBoxLayout*SegmentationMethodeWidgetLayout;

    QWidget *SegmentationMethodeLeftWidget;

    QWidget *SegmentationMethodeRightWidget;
    QVBoxLayout *SegmentationMethodeRightWidgetLayout;
    CPushButton* thresholdSegementationButton;
    CPushButton* adaptiveRegionGrowingSegementationButton;
    CPushButton* boostSegementationButton;
    CPushButton* cropVesselSegementationButton;

    QSpacerItem* segementationButtonSpacer;

    QWidget *SegmentationOperationWidget;
    QGridLayout *SegmentationOperationWidgetLayout;

    /*
    QWidget* thresholdSegementationBoard;
    QWidget* adaptiveRegionGrowingSegementationBoard;
    QWidget* boostSegementationBoard;
    QWidget* cropVesselSegementationBoard;
    */

    QWidget* thresholdSegementationBoard;
    QGridLayout*thresholdSegementationBoardLayout;
    PlottingBoard *thresholdSegementationPlottingBoard;//0,0
    QWidget *thresholdSegementationInformationArea;//0,1
    QWidget *thresholdSegementationConfigurationBar;//1,0
    QHBoxLayout * thresholdSegementationConfigurationBarLayout;
    QLineEdit * thresholdInformationLineEdit;
    QPushButton * applyThresholdSegementationButton;

    QWidget *thresholdSegementationControlLabel;//1,1

    QWidget* adaptiveRegionGrowingSegementationBoard;
    QWidget* boostSegementationBoard;
    QWidget* cropVesselSegementationBoard;



    QPixmap *pixmap;

    SegmentationButtonStates segmentationButtonStates;


    Patient *patient;


    void drawBackground();
    void contructIHM();
    void initialisation();
    void setConnections();
    void embellishIHM();
    void printButtonStates();
    void doThresholdSegementationPlottingBoardPlotting();


signals:

public slots:
    void thresholdSegementationButtonHovered();
    void thresholdSegementationButtonClicked();
    void thresholdSegementationButtonReleased();
    void thresholdSegementationButtonLeaved();

    void adaptiveRegionGrowingSegementationButtonHovered();
    void adaptiveRegionGrowingSegementationButtonClicked();
    void adaptiveRegionGrowingSegementationButtonReleased();
    void adaptiveRegionGrowingSegementationButtonLeaved();

    void boostSegementationButtonHovered();
    void boostSegementationButtonClicked();
    void boostSegementationButtonReleased();
    void boostSegementationButtonLeaved();

    void cropVesselSegementationButtonHovered();
    void cropVesselSegementationButtonClicked();
    void cropVesselSegementationButtonReleased();
    void cropVesselSegementationButtonLeaved();

    void thresholdSegementationPlottingBoardTracking(QMouseEvent* event);
    void thresholdSegementationPlottingBoardDoubleClicked(QMouseEvent* event);
    void thresholdSegementationPlottingBoardClicked(QMouseEvent* event);
    void thresholdSegementationPlottingBoardReleased(QMouseEvent* event);

};

#endif // PATIENTWIDGETCONFIGURATIONBOARD_H
