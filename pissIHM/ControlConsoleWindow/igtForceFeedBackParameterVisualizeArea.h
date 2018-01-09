#ifndef FORCEFEEDBACKPARAMETERVISUALIZEAREA_H
#define FORCEFEEDBACKPARAMETERVISUALIZEAREA_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QString>
#include "igtAnalogMotor.h"


class igtForceFeedBackParameterVisualizeArea : public QWidget
{
    Q_OBJECT

public:
    void constructIHM();
    void rotateAngle(int angle);

private:
    igtAnalogMotor *motor;
    QLabel* indicationLabel;
    QProgressBar* progressBar;
    QVBoxLayout *myLayout;
    QHBoxLayout *indicationLabelLayout;
    QString progressBarStyleSheet;

    int width;
    int height;
    QString parameterName;
    QColor *minuteColor;
    QColor *roundColor;

public:
    explicit igtForceFeedBackParameterVisualizeArea(QString parameterName, int width, int height, QColor *minuteColor, QColor *roundColor);

signals:

public slots:
};

#endif // FORCEFEEDBACKPARAMETERVISUALIZEAREA_H
