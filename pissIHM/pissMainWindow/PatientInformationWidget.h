/*! \file       PatientInformationWidget.h
 *  \brief     The widget wich contains the minimum informations to be filled in order to create a patient in the data base.
 *
 *  ................ Description
 *
 *  \author   Cheng WANG
 *  \date     2015.04.15
 *  \version  1.0
****************************************************************************************************************************/

#ifndef PATIENTINFORMATIONWIDGET_H
#define PATIENTINFORMATIONWIDGET_H

#include "PersonnelImageLabel.h"

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QPixmap>
#include <QBitmap>
#include <QMessageBox>
#include <CPushButton.h>

class PatientInformationWidget : public QWidget{

    Q_OBJECT
public:
    explicit PatientInformationWidget(int screen_width, int screen_height);
    ~PatientInformationWidget();

    void init();
    void construct();
    void connections();

    void display(QPoint pos);
    void drawBackground();
    bool patientInformationConfigured();
    QString getPatientImagePath();
    QString getPatientName();
    QString getPatientBirth();
    QString getPatientSex();
    QString getPatientTherapyDate();
    void setParentWidget(QObject *p);


private:

    int screen_width;
    int screen_height;
    QPixmap *pixmap;
    QLabel *nameLabel;
    QLabel *birthdayLabel;
    QLabel *sexualLabel;
    QLineEdit *nameLineEdit;
    QLineEdit *birthdayLineEdit;
    QLineEdit *sexualLineEdit;
    QWidget *informationContainer;
    QGridLayout *informationLayout;

    QSpacerItem *patientSpacerItem;
    PersonnelImageLabel *patientImageLabel;
    QWidget *patientInformationContainer;
    QHBoxLayout *patientLayout;

    QGroupBox *doctorCommentGroupBox;
    QLabel *therapyTimeLabel;
    QTextEdit *commentTextEdit;
    QLabel *leadDoctorLabel;
    QLineEdit *therapyTimeLineEdit;
    QLineEdit *leadDoctorLineEdit;
    QWidget * doctorInformationContainer;
    QGridLayout *doctorInformationLayout;
    QVBoxLayout *doctorLayout;

    CPushButton *cancelButton;
    CPushButton *confirmButton;
    QSpacerItem *ccSpacerItem;
    QWidget *ccContatiner;
    QHBoxLayout *ccLayout;

    QVBoxLayout *mainLayout;
    QFont *globalFontStyle;

    QPoint mouseMovingPosition;
    QPoint mousePosition;
    bool mouseLeftButtonPressed;
    bool displaying;
    bool informationConfigured;
   QObject *parent;

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

signals:

public slots:
    void confirm();
    void cancel();
};

#endif // PATIENTINFORMATIONWIDGET_H
