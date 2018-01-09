#include "igtForceFeedBackParameterVisualizeArea.h"

igtForceFeedBackParameterVisualizeArea::igtForceFeedBackParameterVisualizeArea(QString parameterName, int width, int height, QColor *minuteColor, QColor *roundColor)
{
    this->width = width;
    this->height = height;
    this->parameterName = parameterName;
    this->minuteColor = minuteColor;
    this->roundColor = roundColor;

    this->setStyleSheet("background-color:transparent; border: 0px solid rgb(251,95,33)");
    this->progressBarStyleSheet = "QProgressBar {border: 0px solid aliceBlue; color: aliceBlue}"
                                  "QProgressBar::chunk {background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 rgb(251,95,33), stop: 1 rgb(251,95,33));}";


    this->constructIHM();

}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief ForceFeedBackParameterVisualizeArea::rotate
//! \param angle
//!
void igtForceFeedBackParameterVisualizeArea::rotateAngle(int angle){
    int ret;
    motor->rotateAngle(angle);
    if(angle < 0){
        ret = -angle;
    }
    this->progressBar->setValue(ret);

}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief ForceFeedBackParameterVisualizeArea::constructIHM
//!
void igtForceFeedBackParameterVisualizeArea::constructIHM(){

    motor = new igtAnalogMotor(parameterName, minuteColor, roundColor, this->height*0.8, this->height*0.8);

    progressBar = new QProgressBar();
    progressBar->setOrientation(Qt::Horizontal);
    progressBar->setAlignment(Qt::AlignHCenter);
    progressBar->setFixedHeight(this->height*0.1);
    progressBar->setFont(QFont("Segoe UI", 12, QFont::AnyStyle, true));
    progressBar->setStyleSheet(this->progressBarStyleSheet);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(30);

    indicationLabel = new QLabel();
    indicationLabel->setFixedHeight(this->height*0.2);
    indicationLabelLayout = new QHBoxLayout(indicationLabel);
    indicationLabelLayout->addWidget(progressBar);
    indicationLabelLayout->setSpacing(0);
    indicationLabelLayout->setMargin(0);

    myLayout = new QVBoxLayout(this);
    myLayout->addWidget(motor);
    myLayout->addWidget(indicationLabel);
    myLayout->setSpacing(0);
    myLayout->setMargin(0);
}
