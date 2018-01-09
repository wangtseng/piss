#include "PatientItem.h"


/**
 * @brief PatientItem::PatientItem
 * @param appWidth
 * @param appHeight
 * @param caracterStyle
 */
PatientItem::PatientItem(int appWidth, int appHeight, QFont *caracterStyle, QString globalFontColor)
{
    this->appWidth = appWidth;
    this->appHeight = appHeight;
    this->caracterStyle = caracterStyle;
    this->globalFontColor = globalFontColor;

    this->consructIHM();
}

//! ------------------------------------------------------------------------------------------
//!
//! \brief PatientItem::consructIHM
//!
void PatientItem::consructIHM(){

    this->photoLabelStyleSheet = "border: 0px solid aliceBlue;border-radius: 0px;"
                                 "padding: 2 2px;background-color: transparent; color:"+this->globalFontColor;

    this->labelStyleSheet = "border: 0.5px solid lightgray; border-radius: 0px; "
                            "padding: 2 2px; background-color: transparent; color:"+this->globalFontColor;

    this->patientInfoOne = new QLabel();
    this->patientInfoOne->setStyleSheet("border-image: url(:/images/xueguan.png);background:transparent");
    this->patientInfoTwo = new QLabel();
    this->patientInfoTwo->setStyleSheet("border-image: url(:/images/Brain.png);background:transparent");
    this->patientInfoThree = new QLabel();
    this->patientInfoThree->setStyleSheet("border-image: url(:/images/notebook.png);background:transparent");

    this->patientInfoWidget = new QWidget();
    this->patientInfoWidget->setFixedHeight(this->appHeight*0.03);
    this->patientInfoLayout = new QHBoxLayout(patientInfoWidget);
    this->patientInfoLayout->addWidget(patientInfoOne);
    this->patientInfoLayout->addWidget(patientInfoTwo);
    this->patientInfoLayout->addWidget(patientInfoThree);
    this->patientInfoLayout->setSpacing(0);
    this->patientInfoLayout->setMargin(0);

    this->patientNameLabel = new QLabel();
    this->patientNameLabel->setFont(*this->caracterStyle);
    this->patientNameLabel->setFixedHeight(this->appHeight*0.166*0.16);
    this->patientNameLabel->setStyleSheet("background-color: rgba(0, 0, 0, 100);");
    this->patientNameLabel->setAlignment(Qt::AlignCenter);

    this->patientPhotoBlankArea = new QWidget();

    this->patientPhotoLabel = new QLabel();
    this->patientPhotoLabel->setStyleSheet(this->photoLabelStyleSheet);
    this->patientPhotoLayout = new QVBoxLayout(patientPhotoLabel);
    this->patientPhotoLayout->addWidget(patientNameLabel);
    this->patientPhotoLayout->addWidget(patientPhotoBlankArea);
    this->patientPhotoLayout->setSpacing(0);
    this->patientPhotoLayout->setMargin(0);

    this->patientItemLayout = new QVBoxLayout(this);
    this->patientItemLayout->addWidget(patientPhotoLabel);
    this->patientItemLayout->addWidget(patientInfoWidget);
    this->patientItemLayout->setSpacing(0);
    this->patientItemLayout->setMargin(0);
}

//! ------------------------------------------------------------------------------------------
//!
//! \brief PatientItem::getPatientPhoto
//! \return
//!
QLabel* PatientItem::getPatientPhoto(){
    return this->patientPhotoLabel;
}

//! ------------------------------------------------------------------------------------------
//!
//! \brief PatientItem::getPatientName
//! \return
//!
QLabel* PatientItem::getPatientName(){
    return this->patientNameLabel;
}

//! ------------------------------------------------------------------------------------------
//!
//! \brief PatientItem::~PatientItem
//!
PatientItem::~PatientItem(){

}

