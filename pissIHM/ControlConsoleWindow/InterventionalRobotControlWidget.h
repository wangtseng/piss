#ifndef INTERVENTIONALROBOTCONTROLWIDGET_H
#define INTERVENTIONALROBOTCONTROLWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QPixmap>
#include <QBrush>
#include <QMessageBox>
#include <QBitmap>
#include <QScreen>
#include <QApplication>
#include <QDebug>
#include <QLineEdit>
#include <QProgressBar>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include "igtAnalogMotor.h"
#include "SystemDispatcher.h"
#include "igtForceFeedBackParameterVisualizeArea.h"
#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include "igtShowVelocityIsTask.h"
#include "igtShowForceTorqueValuesTask.h"
#include "Patient.h"
#include "igtNDIPosition.h"


/**
 * @brief The ControlConsoleWindow class
 */
class InterventionalRobotControlWidget : public QWidget
{
    Q_OBJECT

private:
    QVBoxLayout *controlConsoleWindowLayout;
    QGridLayout* indicationWindowLayout;
    QVBoxLayout* realTimeTrackingWindowLayout;
    QVBoxLayout* forceFeedBackTrackingAreaLayout;
    QHBoxLayout* interventionalToolControlAreaLayout;
    QGridLayout* forceFeedBackParameterVisualizeAreaLayout;
    QGridLayout* ndiVisualiseAreaLayout;

    QPushButton* localPCIndicationLabel;
    QLabel* localIpLabel;
    QLabel* localListeningPort;
    QLabel* localPCControlLabel;
    QLabel *pressureTestLabel;
    QPushButton* stopAllButton;
    QPushButton* motorProgressResetButton;

    QPushButton* remotePCIndicationLabel;
    QLineEdit* remoteIpLabel;
    QLineEdit* remoteListeningPort;

    QGroupBox* ndiVisualiseArea;
    QGroupBox * moteurSwichingArea;
    QGridLayout *moteurSwichingAreaLayout;
    QLabel* moteurHandlingIndication;
    QLabel *contrastLabel;
    QLabel *contrastSpeedLabel;
    QLineEdit *contrastSpeedLineEdit;
    QLabel *backGripperLabel;
    QPushButton *injectButton;

    QComboBox * moteurSwitching;
    QLineEdit *contrastLineEdit;
    QProgressBar *volumeProgressBar;

    QLabel* abscissa_pos;
    QLabel* ordinate_pos;
    QLabel* iso_pos;
    QLabel* abscissa_indication_pos;
    QLabel* ordinate_indication_pos;
    QLabel* iso_indication_pos;

    QLabel* forceFeedBackParametersVisualizeArea;

    igtForceFeedBackParameterVisualizeArea *fxVisualizeArea;
    igtForceFeedBackParameterVisualizeArea *fyVisualizeArea;
    igtForceFeedBackParameterVisualizeArea *fzVisualizeArea;
    igtForceFeedBackParameterVisualizeArea *txVsualizeArea;
    igtForceFeedBackParameterVisualizeArea *tyVisualizeArea;
    igtForceFeedBackParameterVisualizeArea *tzVisualizeArea;

    QLabel *forceFeedBackTrackingArea;
    QLabel *interventionalToolIndicationBar;
    QLabel *interventionalToolControlArea;
    QLabel *rotatingArea;
    QLabel *rotationControlBar;

    QHBoxLayout*rotationControlBarLayout;

    QLabel *advancementControlArea;
    bool rotatingOrientationFlag;
    QPushButton *rotatingRight;
    QPushButton *rotatingLeft;
    igtAnalogMotor *rotateMotor;

    QPushButton *rotateSpeedAddition;
    QPushButton *rotateSpeedSubtraction;

    QVBoxLayout *rotatingAreaLayout;
    QGridLayout *rotateSpeedLayout;
    QGridLayout *advancementControlAreaLayout;
    QPushButton *advancementControlUp;
    QPushButton *advancementControlDown;
    QPushButton *advancementControlLeft;
    QPushButton *advancementControlRight;
    QLabel *advancementControlShow;
    QLabel *forceFeedBackTrackingIndicationBar;


    QProgressBar *progressBufferIndicationBar;
    QProgressBar *rotateBufferIndicationBar;    

    QFrame *realTimeTrackingWindow;
    QFrame *remoteControlArea;

    int width;
    int height;
    QString workspaceColor;
    QTime* surgeryTime;

    QString configuratonFilePath;
    QFont *caracterStyle;

    bool isLocalPCIndicationLabelClicked;
    bool isRemotePCIndicationLabelClicked;

    SystemDispatcher *controller;
    QColor *minuteColor;
    QColor *roundColor;
    QColor *forceTorqueRoundColor;
    QColor *forceTorqueColor;

    QTimer *parameterVisualizeTimer;
    int tigtag;

    QTimer *parameterProgressMotorTimer;
    bool symbol;

    QTimer *parameterProgressVisualizeTimer;

    QTimer *parameterRotateVisualizeTimer;
    
    QTimer *parameterRotateMotorTimer;

    QTimer *omegaEchantionnageTimer;

    QTimer *motivateConnectTimer;

    bool fetchOmegaParameter;

    QPixmap *pixmap;

    igtShowVelocityIsTask *showAdvancementTargetVelocityTask;
    igtShowForceTorqueValuesTask *showForceTorqueValuesTask;
    Patient *patientHandling;

public:
    void initVariable();
    void displayWindow();
    void constructionIHM();
    void constructCommunicationArea();
    void constructInterventionalToolControlArea();
    void constructForceFeedBackVisualizeArea();
    void setConnections();
    void drawBackground();

    void connectMotivateOrBack(bool MB);
    void showForceTorqueValues();
    void setPatientHandling(Patient *patientHandling);
    void showNdiValues();
    void showGuidewireMovement();

signals:
    void missionAccomplishiment();

public slots:
    void stopAll();
    void speedAddition();
    void speedSubtraction();
    void advancementControlUpPressed();
    void advancementControlUpReleased();
    void advancementControlDownPressed();
    void advancementControlDownReleased();
    void rotatingSpeedAdditionPressed();
    void rotatingSpeedAdditionReleased();
    void rotatingSpeedSubtractionPressed();
    void rotatingSpeedSubtractionReleased();
    void rotatingLeftPressed();
    void rotatingLeftReleased();
    void rotatingRightPressed();
    void rotatingRightReleased();
    void localPCIndicationLabelClicked();
    void remotePCIndicationLabelClicked();
    void updateRotateMotorBuffer();
    void updateRotateCommandBuffer();
    void visualizeParameterInContext();
    void updateProgressMotorBuffer();
    void updateProgressCommandBuffer();
    void motorProgressReset();

    void getSelfIP(QString IP);
    void connectBack(QString remoteIP);

    void omegaSampling();
    void connectera();

    void controlObjectChanged(const QString & obj);
    void pushContrastMedia();
    void pullBackContrastMedia();

public:
    explicit InterventionalRobotControlWidget(SystemDispatcher *controller, int w, int h);

};

#endif // CONTROLCONSOLEWINDOW_H
