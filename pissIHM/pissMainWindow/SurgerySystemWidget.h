#ifndef SURGERYSYSTEMWIDGET_H
#define SURGERYSYSTEMWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QBrush>
#include <QBitmap>
#include <QLabel>
#include <QLineEdit>

#include "CommunicationStatesContainer.h"
#include "SystemComponentsContainer.h"
#include "SystemDispatcher.h"
#include "AlgorithmTestPlatform.h"


/**
 * @brief The SurgerySystemWidget class
 */
class SurgerySystemWidget:public QWidget
{
    Q_OBJECT

private:
    int width;
    int height;

    QString workspaceColor;
    QWidget *surgerySystemControlBar;
    QWidget *surgerySystemComponents;
    QPushButton *startServerButton;
    QPushButton *stopServerButton;
    QSpacerItem *surgerySystemControlBarItem;
    QHBoxLayout *surgerySystemControlBarLayout;
    QVBoxLayout *myLayout;
    QHBoxLayout *surgerySystemComponentsLayout;
    SystemComponentsContainer *systemComponentsContainer;
    CommunicationStatesContainer *communicationStatesContainer;
    SystemDispatcher* systemDispatcher;
    AlgorithmTestPlatform* algorithmTestPlatform;

public:
    void initVariable();
    void constructIHM();
    void setConnections();
    void closeSystem();
    void drawBackground();
//    void setSelfInfo(QString addr, int port);

public slots:
    void onStartServerButtonClicked();
    void onStopServerButtonClicked();
    void testConnectBack(QString addr, int port);

public:
     explicit SurgerySystemWidget(int width,
                                  int height,
                                  SystemDispatcher* systemDispatcher,
                                  AlgorithmTestPlatform* algorithmTestPlatform,
                                  QString workspaceColor);
};

#endif // SURGERYSYSTEMWIDGET_H
