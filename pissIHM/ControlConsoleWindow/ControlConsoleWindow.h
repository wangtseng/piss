#ifndef CONTROLCONSOLEWINDOW_H
#define CONTROLCONSOLEWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QPixmap>
#include <QBrush>
#include <QMessageBox>
#include <QBitmap>
#include "SystemDispatcher.h"
#include "AlgorithmTestPlatform.h"
#include "Patient.h"

#include "InterventionalRobotControlWidget.h"
#include "pissCanvas3D.h"

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QThread>
#include <QDesktopWidget>
#include <QRect>
#include <QPainter>


/**
 * @brief The ControlConsoleWindow class
 */
class ControlConsoleWindow : public QWidget
{
    Q_OBJECT

private:
    QVBoxLayout *controlConsoleWindowLayout;
    QHBoxLayout* indicationWindowLayout;
    QHBoxLayout* realTimeTrackingWindowLayout;

    QLabel* guidewareTrackingIndication;
    QLabel* ballonStentTranckingArea;
    QPushButton* missionAccomplishimentButton;

    QFrame *realTimeTrackingWindow;
    QFrame *indicationWindow;

    InterventionalRobotControlWidget *interventionalRobotControl;

    int width;
    int height;
    QString workspaceColor;
    SystemDispatcher* systemDispatcher;
    AlgorithmTestPlatform *algorithmTestPlatform;
    QTime* surgeryTime;

    QString configuratonFilePath;
    QFont *caracterStyle;
    Patient *patientHandling;
    int clickCounter;

public:
    void initVariable();
    void displayWindow();
    void constructionIHM();
    void setConnections();
    void drawBackground();
    void setPatientHandling(Patient *patientHandling);
    void update();

signals:
    void missionAccomplishiment(QString path);

public slots:
    void onMissionAccomplishimentButtonClicked();

public:
    explicit ControlConsoleWindow(QRect rect,
                                  QTime* surgeryTime,
                                  SystemDispatcher* systemDispatcher,
                                  AlgorithmTestPlatform *algorithmTestPlatform,
                                  QString workspaceColor,
                                  QString configuratonFilePath);

};

#endif // CONTROLCONSOLEWINDOW_H
