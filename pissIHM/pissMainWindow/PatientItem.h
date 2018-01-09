#ifndef PATIENTITEM_H
#define PATIENTITEM_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class PatientItem : public QWidget
{
    Q_OBJECT
public:
    PatientItem(int appWidth, int appHeight, QFont *caracterStyle, QString globalFontColor);
    ~PatientItem();

    void consructIHM();
    QLabel* getPatientPhoto();
    QLabel* getPatientName();

private:
    int appWidth;
    int appHeight;

    QFont *caracterStyle;
    QString globalFontColor;

    QString labelStyleSheet;
    QString photoLabelStyleSheet;

    QVBoxLayout* patientItemLayout;
    QVBoxLayout* patientPhotoLayout;
    QHBoxLayout* patientInfoLayout;

    QWidget* patientInfoWidget;
    QWidget* patientPhotoBlankArea;


    QLabel* patientInfoOne;
    QLabel* patientInfoTwo;
    QLabel* patientInfoThree;
    QLabel* patientPhotoLabel;
    QLabel* patientNameLabel;

signals:

public slots:
};

#endif // PATIENTITEM_H
