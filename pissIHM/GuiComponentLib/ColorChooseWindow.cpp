#include "ColorChooseWindow.h"

ColorChooseWindow::ColorChooseWindow(QDialog *parent) : QDialog(parent)
{

    this->embellishIHM();

    this->constructIHM();

    this->setConnections();

    this->variableInitialisation();
}

ColorChooseWindow::~ColorChooseWindow(){}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::confirmed
//! \return
//!
bool ColorChooseWindow::colorChosen(){
    return this->isConfirmed;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::getRedValue
//! \return
//!
int ColorChooseWindow::getRedValue(){
    return this->redValue;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::getGreenValue
//! \return
//!
int ColorChooseWindow::getGreenValue(){
    return this->greenValue;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::getBlueValue
//! \return
//!
int ColorChooseWindow::getBlueValue(){
    return this->blueValue;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::colorUpdate
//!
void ColorChooseWindow::colorUpdate(){
    colorDescription = "rgb("+redValueLabel->text() + "," + greenValueLabel->text() + "," + blueValueLabel->text() + ")";
    colorDisplayer->setStyleSheet("background-color:" + colorDescription);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::setRedValue
//! \param redValue
//!
void ColorChooseWindow::setRedValue(int redValue){
    this->redValue = redValue;
    this->redValueLabel->setText(QString::number(this->redValue));
    this->colorUpdate();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::setGreenValue
//! \param greenValue
//!
void ColorChooseWindow::setGreenValue(int greenValue){
    this->greenValue = greenValue;
    this->greenValueLabel->setText(QString::number(this->greenValue));
    this->colorUpdate();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::setBlueValue
//! \param blueValue
//!
void ColorChooseWindow::setBlueValue(int blueValue){
    this->blueValue = blueValue;
    this->blueValueLabel->setText(QString::number(this->blueValue));
    this->colorUpdate();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::confirmed
//!
void ColorChooseWindow::confirmed(){
    this->isConfirmed = true;
    this->close();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::cancelled
//!
void ColorChooseWindow::cancelled(){
    this->isConfirmed = false;
    this->close();
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::drawBackground
//!
void ColorChooseWindow::drawBackground(){
    pixmap = new QPixmap(":/images/background_darkBlue.png");
    QPalette p =  this->palette();
    p.setBrush(QPalette::Background, QBrush(pixmap->scaled(QSize(350, 300), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(p);
    this->setMask(pixmap->mask());
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::setConnections
//!
void ColorChooseWindow::setConnections(){
    connect(confirmButton,SIGNAL(clicked()), this, SLOT(confirmed()));
    connect(cancelButton,SIGNAL(clicked()), this, SLOT(cancelled()));

    connect(redSlider, SIGNAL(valueChanged(int)), this, SLOT(setRedValue(int)));
    connect(greenSlider, SIGNAL(valueChanged(int)), this, SLOT(setGreenValue(int)));
    connect(blueSlider, SIGNAL(valueChanged(int)), this, SLOT(setBlueValue(int)));
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::VariableInitialisation
//!
void ColorChooseWindow::variableInitialisation(){
    redValue = 0;
    greenValue = 0;
    blueValue = 0;
    isConfirmed = false;
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::IHMInitialisation
//!
void ColorChooseWindow::constructIHM(){
    indicationLabel = new QLabel("Set color by adjust RGB values");
    indicationLabel->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, false));
    indicationLabel->setStyleSheet("color: AliceBlue");
    indicationLabel->setFixedHeight(40);

    colorConfigurationFrame = new QFrame();
    colorConfigurationFrame->setFixedHeight(150);
    colorConfigurationFrameLayout = new QHBoxLayout(colorConfigurationFrame);

    colorAjustmentArea = new QWidget();
    colorAjustmentAreaLayout = new QGridLayout(colorAjustmentArea);

    redLabel = new QLabel("Red: ");
    redLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    redLabel->setStyleSheet("color: AliceBlue");

    greenLabel = new QLabel("Green: ");
    greenLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    greenLabel->setStyleSheet("color: AliceBlue");

    blueLabel = new QLabel("Blue: ");
    blueLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    blueLabel->setStyleSheet("color: AliceBlue");

    redValueLabel= new QLabel();
    redValueLabel->setFixedSize(50,50);
    redValueLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    redValueLabel->setStyleSheet("color: AliceBlue");
    redValueLabel->setText("50");

    greenValueLabel = new QLabel();
    greenValueLabel->setFixedSize(50,50);
    greenValueLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    greenValueLabel->setStyleSheet("color: AliceBlue");
    greenValueLabel->setText("50");

    blueValueLabel = new QLabel();
    blueValueLabel->setFixedSize(50,50);
    blueValueLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    blueValueLabel->setStyleSheet("color: AliceBlue");
    blueValueLabel->setText("50");

    redSlider = new QSlider(Qt::Horizontal);
    redSlider->setMinimum(0);
    redSlider->setMaximum(255);

    greenSlider = new QSlider(Qt::Horizontal);
    greenSlider->setMinimum(0);
    greenSlider->setMaximum(255);

    blueSlider = new QSlider(Qt::Horizontal);
    blueSlider->setMinimum(0);
    blueSlider->setMaximum(255);

    redSlider->setValue(50);
    greenSlider->setValue(50);
    blueSlider->setValue(50);

    colorAjustmentAreaLayout->addWidget(redLabel,0,0);
    colorAjustmentAreaLayout->addWidget(greenLabel,1,0);
    colorAjustmentAreaLayout->addWidget(blueLabel,2,0);
    colorAjustmentAreaLayout->addWidget(redSlider,0,1);
    colorAjustmentAreaLayout->addWidget(greenSlider,1,1);
    colorAjustmentAreaLayout->addWidget(blueSlider,2,1);
    colorAjustmentAreaLayout->addWidget(redValueLabel,0,2);
    colorAjustmentAreaLayout->addWidget(greenValueLabel,1,2);
    colorAjustmentAreaLayout->addWidget(blueValueLabel,2,2);
    //colorAjustmentAreaLayout->setSpacing(0);
    //colorAjustmentAreaLayout->setMargin(0);

    colorDisplayArea = new QWidget();
    colorDisplayAreaLayout = new QVBoxLayout(colorDisplayArea);
    colorDisplayAreaIndication = new QFrame();
    colorDisplayer = new QLabel();
    colorDisplayer->setFixedWidth(100);
    //colorDisplayer->setFixedSize(100,100);

    //colorDisplayAreaLayout->addWidget(colorDisplayAreaIndication);
    colorDisplayAreaLayout->addWidget(colorDisplayer);
    colorDisplayAreaLayout->setSpacing(0);
    colorDisplayAreaLayout->setMargin(0);

    colorConfigurationFrameLayout->addWidget(colorAjustmentArea);
    colorConfigurationFrameLayout->addWidget(colorDisplayArea);
    colorConfigurationFrameLayout->setSpacing(0);
    colorConfigurationFrameLayout->setMargin(0);

    confirmationFrame = new QFrame();
    confirmationFrameLayout = new QHBoxLayout(confirmationFrame);
    confirmButton = new QPushButton("confirm");
    confirmButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, false));
    confirmButton->setStyleSheet("background-color:transparent; color: AliceBlue");

    cancelButton = new QPushButton("cancel");
    cancelButton->setFont(QFont("Segoe UI", 10, QFont::AnyStyle, false));
    cancelButton->setStyleSheet("background-color:transparent;color: AliceBlue");
    confirmationFrameLayout->addWidget(confirmButton);
    confirmationFrameLayout->addWidget(cancelButton);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(indicationLabel);
    mainLayout->addWidget(colorConfigurationFrame);
    mainLayout->addWidget(confirmationFrame );
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
}

//--------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief ColorChooseWindow::embellishIHM
//!
void ColorChooseWindow::embellishIHM(){
    this->resize(350, 250);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setWindowOpacity(0.96);
    this->setMouseTracking(true);
    this->setAutoFillBackground(true);
    this->drawBackground();
}
