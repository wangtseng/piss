#ifndef SUPERVISEWINDOW_H
#define SUPERVISEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QBitmap>
#include <QPixmap>
#include <QPalette>

class SuperviseWindow : public QWidget
{
    Q_OBJECT
public:
    SuperviseWindow(int appWidth, int appHeight);
    void initVariable();
    void constructIHM();
    void setConnections();
    void drawBackground();

private:
    int appWidth;
    int appHeight;

    QPixmap *pixmap;

    QGridLayout *mainLayout;
    QVBoxLayout *patientInfoLayout;

    QLabel *cArmSuperviseWindow;
    QLabel *pusherMechanismWindow;
    QLabel *patientInteractorWindow;
    QLabel *electrocardiogramDisplay;

    QWidget *patientInfoWidget;
    QWidget *vitalSignsWidget;

signals:

public slots:
};

#endif // SUPERVISEWINDOW_H
