#include "PatientWidgetConfigurationBoard.h"


/**
 * @brief PatientWidgetConfigurationBoard::PatientWidgetConfigurationBoard
 * @param _patient
 */
PatientWidgetConfigurationBoard::PatientWidgetConfigurationBoard() : QWidget(){


    this->embellishIHM();
    this->contructIHM();
    this->setConnections();

    thresholdSegementationButton->pressed();
    printButtonStates();
    doThresholdSegementationPlottingBoardPlotting();
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::~PatientWidgetConfigurationBoard
//!
PatientWidgetConfigurationBoard::~PatientWidgetConfigurationBoard(){

}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::setPatientHandling
//! \param _patient
//!
void PatientWidgetConfigurationBoard::setPatientHandling(Patient *_patient){
    this->patient = _patient;
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::printButtonStates
//!
void PatientWidgetConfigurationBoard::printButtonStates(){
//    qDebug()<<segmentationButtonStates.thresholdSegementationState;
//    qDebug()<<segmentationButtonStates.adaptiveRegionGrowingSegementationState;
//    qDebug()<<segmentationButtonStates.boostSegementationState;
//    qDebug()<<segmentationButtonStates.cropVesselSegementationState;
}


//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::thresholdSegementationButtonHovered
//!
void PatientWidgetConfigurationBoard::thresholdSegementationButtonHovered(){
    if(!segmentationButtonStates.thresholdSegementationState){
        thresholdSegementationButton->setStyleSheet("border: 1px solid grey;  border-radius: 0px; background-color: gainsboro;  min-width: 0px; color: AliceBlue");
    }
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::thresholdSegementationButtonClicked
//!
void PatientWidgetConfigurationBoard::thresholdSegementationButtonClicked(){
    if(!segmentationButtonStates.thresholdSegementationState){
        thresholdSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: AliceBlue " );
    }
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::thresholdSegementationButtonReleased
//!
void PatientWidgetConfigurationBoard::thresholdSegementationButtonReleased(){
    if(!segmentationButtonStates.thresholdSegementationState){
        thresholdSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: grey;  min-width: 0px; color: AliceBlue  "  );
        adaptiveRegionGrowingSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey  "  );
        boostSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey  "  );
        cropVesselSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey  "  );

        thresholdSegementationButton->setFont(QFont("Segoe UI",10, QFont::Helvetica, true));
        adaptiveRegionGrowingSegementationButton->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));
        boostSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
        cropVesselSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

        segmentationButtonStates.thresholdSegementationState = true;
        segmentationButtonStates.adaptiveRegionGrowingSegementationState = false;
        segmentationButtonStates.boostSegementationState = false;
        segmentationButtonStates.cropVesselSegementationState = false;
        printButtonStates();

        thresholdSegementationBoard->setFixedSize(620,450);
        adaptiveRegionGrowingSegementationBoard->setFixedSize(0,0);
        boostSegementationBoard->setFixedSize(0,0);
        cropVesselSegementationBoard->setFixedSize(0,0);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::thresholdSegementationButtonLeaved
//!
void PatientWidgetConfigurationBoard::thresholdSegementationButtonLeaved(){
    if(segmentationButtonStates.thresholdSegementationState){
        thresholdSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: grey;  min-width: 0px; color: AliceBlue");
    }
    else{
        thresholdSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey " );
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::adaptiveRegionGrowingSegementationButtonHovered
//!
void PatientWidgetConfigurationBoard::adaptiveRegionGrowingSegementationButtonHovered(){
    if(!segmentationButtonStates.adaptiveRegionGrowingSegementationState){
        adaptiveRegionGrowingSegementationButton->setStyleSheet("border: 1px solid grey;  border-radius: 0px; background-color: gainsboro;  min-width: 0px; color: AliceBlue");
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::adaptiveRegionGrowingSegementationButtonClicked
//!
void PatientWidgetConfigurationBoard::adaptiveRegionGrowingSegementationButtonClicked(){
    if(!segmentationButtonStates.adaptiveRegionGrowingSegementationState){
        adaptiveRegionGrowingSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: AliceBlue  "  );
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::adaptiveRegionGrowingSegementationButtonReleased
//!
void PatientWidgetConfigurationBoard::adaptiveRegionGrowingSegementationButtonReleased(){
    if(!segmentationButtonStates.adaptiveRegionGrowingSegementationState){
        thresholdSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey" );
        adaptiveRegionGrowingSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: grey;  min-width: 0px; color: AliceBlue");
        boostSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey");
        cropVesselSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey");

        thresholdSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
        adaptiveRegionGrowingSegementationButton->setFont(QFont("Segoe UI",10, QFont::Helvetica, true));
        boostSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
        cropVesselSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

        segmentationButtonStates.thresholdSegementationState = false;
        segmentationButtonStates.adaptiveRegionGrowingSegementationState = true;
        segmentationButtonStates.boostSegementationState = false;
        segmentationButtonStates.cropVesselSegementationState = false;
        printButtonStates();
        thresholdSegementationBoard->setFixedSize(0,0);
        adaptiveRegionGrowingSegementationBoard->setFixedSize(620,450);
        boostSegementationBoard->setFixedSize(0,0);
        cropVesselSegementationBoard->setFixedSize(0,0);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::adaptiveRegionGrowingSegementationButtonLeaved
//!
void PatientWidgetConfigurationBoard::adaptiveRegionGrowingSegementationButtonLeaved(){
    if(segmentationButtonStates.adaptiveRegionGrowingSegementationState){
        adaptiveRegionGrowingSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: grey;  min-width: 0px; color: AliceBlue");
    }
    else{
        adaptiveRegionGrowingSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey " );
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::boostSegementationButtonHovered
//!
void PatientWidgetConfigurationBoard::boostSegementationButtonHovered(){
    if(!segmentationButtonStates.boostSegementationState){
        boostSegementationButton->setStyleSheet("border: 1px solid grey;  border-radius: 0px; background-color: gainsboro;  min-width: 0px; color: AliceBlue");
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::boostSegementationButtonClicked
//!
void PatientWidgetConfigurationBoard::boostSegementationButtonClicked(){
    if(!segmentationButtonStates.boostSegementationState){
        boostSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: AliceBlue  "  );
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::boostSegementationButtonReleased
//!
void PatientWidgetConfigurationBoard::boostSegementationButtonReleased(){
    if(!segmentationButtonStates.boostSegementationState){
        thresholdSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey" );
        adaptiveRegionGrowingSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey");
        boostSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: grey;  min-width: 0px; color: AliceBlue");
        cropVesselSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey");

        thresholdSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
        adaptiveRegionGrowingSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
        boostSegementationButton->setFont(QFont("Segoe UI",10, QFont::Helvetica, true));
        cropVesselSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));

        segmentationButtonStates.thresholdSegementationState = false;
        segmentationButtonStates.adaptiveRegionGrowingSegementationState = false;
        segmentationButtonStates.boostSegementationState = true;
        segmentationButtonStates.cropVesselSegementationState = false;
        printButtonStates();
        thresholdSegementationBoard->setFixedSize(0,0);
        adaptiveRegionGrowingSegementationBoard->setFixedSize(0,0);
        boostSegementationBoard->setFixedSize(620,450);
        cropVesselSegementationBoard->setFixedSize(0,0);
    }
}

void PatientWidgetConfigurationBoard::boostSegementationButtonLeaved(){
    if(segmentationButtonStates.boostSegementationState){
        boostSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: grey;  min-width: 0px; color: AliceBlue");
    }
    else{
        boostSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey " );
    }
}

void PatientWidgetConfigurationBoard::cropVesselSegementationButtonHovered(){
    if(!segmentationButtonStates.cropVesselSegementationState){
        cropVesselSegementationButton->setStyleSheet("border: 1px solid grey;  border-radius: 0px; background-color: gainsboro;  min-width: 0px; color: AliceBlue");
    }
}

void PatientWidgetConfigurationBoard::cropVesselSegementationButtonClicked(){
    if(!segmentationButtonStates.cropVesselSegementationState){
        cropVesselSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: AliceBlue" );
    }
}

void PatientWidgetConfigurationBoard::cropVesselSegementationButtonReleased(){
    if(!segmentationButtonStates.cropVesselSegementationState){
        thresholdSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey" );
        adaptiveRegionGrowingSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey");
        boostSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey");
        cropVesselSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: grey;  min-width: 0px; color: AliceBlue");

        thresholdSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
        adaptiveRegionGrowingSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
        boostSegementationButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, true));
        cropVesselSegementationButton->setFont(QFont("Segoe UI",10, QFont::Helvetica, true));

        segmentationButtonStates.thresholdSegementationState = false;
        segmentationButtonStates.adaptiveRegionGrowingSegementationState = false;
        segmentationButtonStates.boostSegementationState = false;
        segmentationButtonStates.cropVesselSegementationState = true;
        printButtonStates();
        thresholdSegementationBoard->setFixedSize(0,0);
        adaptiveRegionGrowingSegementationBoard->setFixedSize(0,0);
        boostSegementationBoard->setFixedSize(0,0);
        cropVesselSegementationBoard->setFixedSize(620,450);
    }
}

void PatientWidgetConfigurationBoard::cropVesselSegementationButtonLeaved(){
    if(segmentationButtonStates.cropVesselSegementationState){
        cropVesselSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: grey;  min-width: 0px; color: AliceBlue");
    }
    else{
        cropVesselSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey " );
    }
}

void PatientWidgetConfigurationBoard::contructIHM(){
    PatientWidgetConfigurationBoardLayout = new QHBoxLayout(this);

    SegmentationOptionWidget = new QWidget();
    SegmentationOptionWidget->setFixedWidth(180);
    SegmentationOptionWidgetLayout = new QVBoxLayout(SegmentationOptionWidget);

    SegmentationObjectWidget = new QWidget();
    SegmentationObjectWidget->setFixedSize(180, 40);
    SegmentationObjectWidgetLayout = new QHBoxLayout(SegmentationObjectWidget);
    SegmentationObjectLabel = new QLabel("Target:");
    SegmentationObjectLabel->setStyleSheet("color: AliceBlue");
    SegmentationObjectLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    SegmentationObjectLabel->setFixedWidth(40);

    segmentationObjectComboBox = new QComboBox();
    segmentationObjectComboBox->setFixedWidth(120);
    segmentationObjectComboBox->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, false));
    segmentationObjectComboBox->setStyleSheet("QComboBox { border: 1px solid aliceblue;  } "
                                              "QComboBox:editable { background-color: rgb(58,89,92); color: darkSeaGreen } ");

    segmentationObjectComboBox->addItem("Blood Vessel");
    segmentationObjectComboBox->addItem("White Matter");
    segmentationObjectComboBox->addItem("Grey matter");

    segmentationObjectComboBoxSpacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

    SegmentationObjectWidgetLayout->addWidget(SegmentationObjectLabel);
    SegmentationObjectWidgetLayout->addWidget(segmentationObjectComboBox);
    SegmentationObjectWidgetLayout->addSpacerItem(segmentationObjectComboBoxSpacer);
    SegmentationObjectWidgetLayout->setSpacing(0);
    SegmentationObjectWidgetLayout->setMargin(2);

    SegmentationMethodeWidget = new QWidget();

    SegmentationMethodeWidgetLayout = new QHBoxLayout(SegmentationMethodeWidget);
    SegmentationMethodeLeftWidget = new QWidget();
    SegmentationMethodeLeftWidget->setFixedWidth(40);

    SegmentationMethodeRightWidget = new QWidget();
    SegmentationMethodeRightWidgetLayout = new QVBoxLayout(SegmentationMethodeRightWidget);

    thresholdSegementationButton = new CPushButton();
    thresholdSegementationButton->setText("Mixture Statistical Model");
    thresholdSegementationButton->setFixedSize(QSize(180, 30));
    thresholdSegementationButton->setFixedWidth(180);
    thresholdSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey  "  );
    thresholdSegementationButton->setFlat(true);
    thresholdSegementationButton->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));

    adaptiveRegionGrowingSegementationButton = new CPushButton();
    adaptiveRegionGrowingSegementationButton->setText(" Adaptive Region Growing");
    adaptiveRegionGrowingSegementationButton->setFixedSize(QSize(180, 30));
    adaptiveRegionGrowingSegementationButton->setFixedWidth(180);
    adaptiveRegionGrowingSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey  "  );
    adaptiveRegionGrowingSegementationButton->setFlat(true);
    adaptiveRegionGrowingSegementationButton->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));

    boostSegementationButton = new CPushButton();
    boostSegementationButton->setText("Boost Segementation");
    boostSegementationButton->setFixedSize(QSize(180, 30));
    boostSegementationButton->setFixedWidth(180);
    boostSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey  "  );
    boostSegementationButton->setFlat(true);
    boostSegementationButton->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));

    cropVesselSegementationButton = new CPushButton();
    cropVesselSegementationButton->setText("Crop Vessel");
    cropVesselSegementationButton->setFixedSize(QSize(180, 30));
    cropVesselSegementationButton->setFixedWidth(180);
    cropVesselSegementationButton->setStyleSheet( "border: 1px solid grey;  border-radius: 0px; background-color: transparent;  min-width: 0px; color: grey  "  );
    cropVesselSegementationButton->setFlat(true);
    cropVesselSegementationButton->setFont(QFont("Segoe UI",10, QFont::AnyStyle, true));

    segementationButtonSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    SegmentationMethodeRightWidgetLayout->addWidget(thresholdSegementationButton);
    SegmentationMethodeRightWidgetLayout->addWidget(adaptiveRegionGrowingSegementationButton);
    SegmentationMethodeRightWidgetLayout->addWidget(boostSegementationButton);
    SegmentationMethodeRightWidgetLayout->addWidget(cropVesselSegementationButton);
    SegmentationMethodeRightWidgetLayout->addSpacerItem(segementationButtonSpacer);

    //SegmentationMethodeWidgetLayout->addWidget(SegmentationMethodeLeftWidget);
    SegmentationMethodeWidgetLayout->addWidget(SegmentationMethodeRightWidget);
    SegmentationMethodeWidgetLayout->setSpacing(0);
    SegmentationMethodeWidgetLayout->setMargin(0);

    SegmentationOptionWidgetLayout->addWidget(SegmentationMethodeWidget);
    SegmentationOptionWidgetLayout->addWidget(SegmentationObjectWidget);
    SegmentationOptionWidgetLayout->setSpacing(0);
    SegmentationOptionWidgetLayout->setMargin(0);

    SegmentationOperationWidget = new QWidget();
    SegmentationOperationWidget->setStyleSheet("background-color:AliceBlue");
    SegmentationOperationWidgetLayout = new QGridLayout(SegmentationOperationWidget);

    thresholdSegementationBoard = new QWidget();
    thresholdSegementationBoardLayout = new QGridLayout(thresholdSegementationBoard);
    thresholdSegementationPlottingBoard = new PlottingBoard();
    thresholdSegementationPlottingBoard->setBoardSize(550,400);
    thresholdSegementationPlottingBoard->setBackgroundColor(240,248,255);
    thresholdSegementationPlottingBoard->setBasePenProperty(QColor(58,89,92), 1);
    thresholdSegementationPlottingBoard->setTickPenProperty(QColor(58,89,92), 1);

    thresholdSegementationInformationArea = new QWidget();
    thresholdSegementationInformationArea->setFixedSize(50, 400);

    thresholdSegementationConfigurationBar = new QWidget();
    thresholdSegementationConfigurationBarLayout = new QHBoxLayout(thresholdSegementationConfigurationBar);
    thresholdInformationLineEdit = new QLineEdit();
    thresholdInformationLineEdit->setFixedWidth(300);
    thresholdInformationLineEdit->setStyleSheet("QLineEdit {border: 2px solid lightGray;border-radius: 10px;padding: 0 8px;background: transparent;selection-background-color: lightBlue;}");
    applyThresholdSegementationButton = new QPushButton("apply");
    applyThresholdSegementationButton->setFlat(1);
    thresholdSegementationConfigurationBarLayout->addWidget(thresholdInformationLineEdit);
    thresholdSegementationConfigurationBarLayout->addWidget(applyThresholdSegementationButton);
    thresholdSegementationConfigurationBarLayout->setMargin(0);
    thresholdSegementationConfigurationBarLayout->setSpacing(0);

    thresholdSegementationControlLabel = new QWidget();

    thresholdSegementationBoardLayout->addWidget(thresholdSegementationPlottingBoard, 0, 0);
    thresholdSegementationBoardLayout->addWidget(thresholdSegementationInformationArea, 0, 1);
    thresholdSegementationBoardLayout->addWidget(thresholdSegementationConfigurationBar, 1, 0);
    thresholdSegementationBoardLayout->addWidget(thresholdSegementationControlLabel, 1, 1);
    thresholdSegementationBoardLayout->setSpacing(0);
    thresholdSegementationBoardLayout->setMargin(0);

    // other segmentation config board
    adaptiveRegionGrowingSegementationBoard = new QWidget();
    adaptiveRegionGrowingSegementationBoard->setFixedSize(550,500);
    adaptiveRegionGrowingSegementationBoard->setStyleSheet("background-color:red");
    boostSegementationBoard = new QWidget();
    boostSegementationBoard->setFixedSize(550,500);
    boostSegementationBoard->setStyleSheet("background-color:yellow");
    cropVesselSegementationBoard = new QWidget();
    cropVesselSegementationBoard->setFixedSize(550,500);
    cropVesselSegementationBoard->setStyleSheet("background-color:blue");

    SegmentationOperationWidgetLayout->addWidget(thresholdSegementationBoard, 0, 0);
    SegmentationOperationWidgetLayout->addWidget(adaptiveRegionGrowingSegementationBoard, 0, 1);
    SegmentationOperationWidgetLayout->addWidget(boostSegementationBoard, 1, 0);
    SegmentationOperationWidgetLayout->addWidget(cropVesselSegementationBoard, 1, 1);
    SegmentationOperationWidgetLayout->setSpacing(0);
    SegmentationOperationWidgetLayout->setMargin(0);
//    thresholdSegementationPlottingBoard->show();
//    adaptiveRegionGrowingSegementationBoard->close();
//    boostSegementationBoard->close();
//    cropVesselSegementationBoard->close();

    PatientWidgetConfigurationBoardLayout->addWidget(SegmentationOptionWidget);
    PatientWidgetConfigurationBoardLayout->addWidget(SegmentationOperationWidget);
    PatientWidgetConfigurationBoardLayout->setSpacing(0);
    PatientWidgetConfigurationBoardLayout->setMargin(0);
}

void PatientWidgetConfigurationBoard::initialisation(){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::setConnections
//!
void PatientWidgetConfigurationBoard::setConnections(){
    this->connect(this->thresholdSegementationButton, SIGNAL(mouseHover()), this, SLOT(thresholdSegementationButtonHovered()));
    this->connect(this->thresholdSegementationButton, SIGNAL(mouseLeftButtonClicked()), this, SLOT(thresholdSegementationButtonClicked()));
    this->connect(this->thresholdSegementationButton, SIGNAL(mouseLeftButtonReleased()), this, SLOT(thresholdSegementationButtonReleased()));
    this->connect(this->thresholdSegementationButton, SIGNAL(mouseLeaved()), this, SLOT(thresholdSegementationButtonLeaved()));

    this->connect(this->adaptiveRegionGrowingSegementationButton, SIGNAL(mouseHover()), this, SLOT(adaptiveRegionGrowingSegementationButtonHovered()));
    this->connect(this->adaptiveRegionGrowingSegementationButton, SIGNAL(mouseLeftButtonClicked()), this, SLOT(adaptiveRegionGrowingSegementationButtonClicked()));
    this->connect(this->adaptiveRegionGrowingSegementationButton, SIGNAL(mouseLeftButtonReleased()), this, SLOT(adaptiveRegionGrowingSegementationButtonReleased()));
    this->connect(this->adaptiveRegionGrowingSegementationButton, SIGNAL(mouseLeaved()), this, SLOT(adaptiveRegionGrowingSegementationButtonLeaved()));

    this->connect(this->boostSegementationButton, SIGNAL(mouseHover()), this, SLOT(boostSegementationButtonHovered()));
    this->connect(this->boostSegementationButton, SIGNAL(mouseLeftButtonClicked()), this, SLOT(boostSegementationButtonClicked()));
    this->connect(this->boostSegementationButton, SIGNAL(mouseLeftButtonReleased()), this, SLOT(boostSegementationButtonReleased()));
    this->connect(this->boostSegementationButton, SIGNAL(mouseLeaved()), this, SLOT(boostSegementationButtonLeaved()));

    this->connect(this->cropVesselSegementationButton, SIGNAL(mouseHover()), this, SLOT(cropVesselSegementationButtonHovered()));
    this->connect(this->cropVesselSegementationButton, SIGNAL(mouseLeftButtonClicked()), this, SLOT(cropVesselSegementationButtonClicked()));
    this->connect(this->cropVesselSegementationButton, SIGNAL(mouseLeftButtonReleased()), this, SLOT(cropVesselSegementationButtonReleased()));
    this->connect(this->cropVesselSegementationButton, SIGNAL(mouseLeaved()), this, SLOT(cropVesselSegementationButtonLeaved()));

    // actions of the mouse's pointer on the plotting board
    this->connect(this->thresholdSegementationPlottingBoard, SIGNAL(mouseDoubleClick(QMouseEvent*)), this, SLOT(thresholdSegementationPlottingBoardDoubleClicked(QMouseEvent*)));
    this->connect(this->thresholdSegementationPlottingBoard, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(thresholdSegementationPlottingBoardTracking(QMouseEvent*)));
    this->connect(this->thresholdSegementationPlottingBoard, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(thresholdSegementationPlottingBoardClicked(QMouseEvent*)));
    this->connect(this->thresholdSegementationPlottingBoard, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(thresholdSegementationPlottingBoardReleased(QMouseEvent*)));
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::thresholdSegementationPlottingBoardTracking
//! \param event
//!
void PatientWidgetConfigurationBoard::thresholdSegementationPlottingBoardTracking(QMouseEvent* event){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::thresholdSegementationPlottingBoardDoubleClicked
//! \param event
//!
void PatientWidgetConfigurationBoard::thresholdSegementationPlottingBoardDoubleClicked(QMouseEvent* event){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::thresholdSegementationPlottingBoardClicked
//! \param event
//!
void PatientWidgetConfigurationBoard::thresholdSegementationPlottingBoardClicked(QMouseEvent* event){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::thresholdSegementationPlottingBoardReleased
//! \param event
//!
void PatientWidgetConfigurationBoard::thresholdSegementationPlottingBoardReleased(QMouseEvent* event){

}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::embellishIHM
//!
void PatientWidgetConfigurationBoard::embellishIHM(){
    this->setFixedSize(800,450);
    this->drawBackground();
    this->setWindowTitle("Segmentation Configuration Board");
    //this->setWindowFlags(Qt::WindowStaysOnTopHint);//Qt::WindowStaysOnTopHint)
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::drawBackground
//!
void PatientWidgetConfigurationBoard::drawBackground(){
    pixmap = new QPixmap(":/images/background.png");
    QPalette p =  this->palette();
    p.setBrush(QPalette::Background, QBrush(pixmap->scaled(QSize(800,600), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(p);
    this->setMask(pixmap->mask());
}

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::display
//! \param pos
//!
void PatientWidgetConfigurationBoard::display(QPoint pos){
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->show();
    this->move(pos.x() + 450, pos.y()-570);
 }

//!--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief PatientWidgetConfigurationBoard::doThresholdSegementationPlottingBoardPlotting
//!
void PatientWidgetConfigurationBoard::doThresholdSegementationPlottingBoardPlotting(){

    /*QVector<HistogramPoint*> frequencies = this->handler->getHistogramFrom(patient->getMRAImage());

    int index = this->thresholdSegementationPlottingBoard->addCurve("Histogram", "grayscale value", "", "cyan", 3);
    this->thresholdSegementationPlottingBoard->doHistogramPlotting(index,frequencies);
    this->thresholdSegementationPlottingBoard->legendSetting(false);
    this->thresholdSegementationPlottingBoard->setAbscissaRange(-100, frequencies.size()+100);
    this->thresholdSegementationPlottingBoard->setOrdinateRange(this->thresholdSegementationPlottingBoard->getOrdinateMinValue()-100000,
                                                                this->thresholdSegementationPlottingBoard->getOrdinateMaxValue()/20);
    this->thresholdSegementationPlottingBoard->setCurveStyle("circle", "black", 6);
    this->thresholdSegementationPlottingBoard->setInteractionTolerated(false, true, true);
    this->thresholdSegementationPlottingBoard->setGridStyle(false, "black", "black", false, "black", "black");*/
}

