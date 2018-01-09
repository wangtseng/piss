#include "PatientInformationWidget.h"
#include "pissMainWindow.h"
#include <QDebug>


/**
 * @brief PatientInformationWidget::PatientInformationWidget
 * @param parent
 */
PatientInformationWidget::PatientInformationWidget(int screen_width, int screen_height) : QWidget()
{
    this->screen_width = screen_width;
    this->screen_height = screen_height;

    this->init();
    this->construct();
    this->connections();
    this->drawBackground();
}

PatientInformationWidget::~PatientInformationWidget(){}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::init
//!
void PatientInformationWidget::init(){
    //! initialise ...
    this->mouseLeftButtonPressed = false;
    this->displaying = false;
    this->informationConfigured = false;

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowOpacity(0.97);
    this->setMouseTracking(true);
    this->setAutoFillBackground(true);

    this->setFixedSize(screen_width*0.35,screen_height*0.5);
    this->setFont( QFont("Segoe UI", 8, QFont::AnyStyle, true));
    this->setStyleSheet("color: AliceBlue");
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::construct
//!
void PatientInformationWidget::construct(){
    nameLabel = new QLabel("Name:");
    nameLabel->setFixedWidth(60);
    birthdayLabel = new QLabel("Birth:");
    sexualLabel  = new QLabel("Sex:");
    nameLineEdit = new QLineEdit("Meng Xianshen");
    nameLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    nameLineEdit->setStyleSheet("QLineEdit {border: 1px solid aliceBlue;border-radius: 0px;padding: 2 2px;background: transparent;selection-background-color: skyblue;}");
    birthdayLineEdit = new QLineEdit("1990.09.03");
    birthdayLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    birthdayLineEdit->setStyleSheet("QLineEdit {border: 1px solid aliceBlue;border-radius: 0px;padding: 2 2px;background: transparent;selection-background-color: skyblue;}");
    sexualLineEdit = new QLineEdit("male");
    sexualLineEdit->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    sexualLineEdit->setStyleSheet("QLineEdit {border: 1px solid aliceBlue;border-radius: 0px;padding: 2 2px;background: transparent;selection-background-color: skyblue;}");
    informationContainer = new QWidget();
    informationLayout = new QGridLayout(informationContainer);
    informationLayout->addWidget(nameLabel, 0, 0);
    informationLayout->addWidget(birthdayLabel, 1, 0);
    informationLayout->addWidget(sexualLabel, 2, 0);
    informationLayout->addWidget(nameLineEdit, 0, 1);
    informationLayout->addWidget(birthdayLineEdit, 1, 1);
    informationLayout->addWidget(sexualLineEdit, 2, 1);
    informationLayout->setSpacing(1);

    patientSpacerItem = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    patientImageLabel = new PersonnelImageLabel();
    patientImageLabel->setFixedWidth(80);
    patientImageLabel->setFixedHeight(100);
    patientImageLabel->setStyleSheet("background-color: lightGray");
    patientImageLabel->setText("...");
    patientInformationContainer = new QWidget();
    patientLayout = new QHBoxLayout(patientInformationContainer);
    patientLayout->addWidget(informationContainer);
    patientLayout->addItem(patientSpacerItem);
    patientLayout->addWidget(patientImageLabel);
    patientLayout->setSpacing(1);

    therapyTimeLabel = new QLabel("Time:");
    therapyTimeLabel->setFixedWidth(60);
    therapyTimeLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    leadDoctorLabel = new QLabel("Doctor:");
    leadDoctorLabel->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
    therapyTimeLineEdit = new QLineEdit("2015.10.10");
    therapyTimeLineEdit->setStyleSheet("QLineEdit {border: 1px solid aliceBlue;border-radius: 0px;padding: 2 2px;background: transparent;selection-background-color: skyblue;}");
    leadDoctorLineEdit = new QLineEdit();
    leadDoctorLineEdit->setStyleSheet("QLineEdit {border: 1px solid aliceBlue;border-radius: 0px;padding: 2 2px;background: transparent;selection-background-color: skyblue;}");
    doctorInformationContainer = new QWidget();
    doctorInformationLayout = new QGridLayout(doctorInformationContainer);
    doctorInformationLayout->addWidget(therapyTimeLabel, 0, 0);
    doctorInformationLayout->addWidget(leadDoctorLabel, 1, 0);
    doctorInformationLayout->addWidget(therapyTimeLineEdit, 0, 1);
    doctorInformationLayout->addWidget(leadDoctorLineEdit, 1, 1);
    doctorInformationLayout->setSpacing(1);

    commentTextEdit = new QTextEdit();
    commentTextEdit->setStyleSheet("border: 1px solid aliceBlue;border-radius: 0px;padding: 2 2px;background: transparent;selection-background-color: skyblue;");
    commentTextEdit->setFont(QFont("Segoe UI", 9, QFont::AnyStyle, false));
    doctorCommentGroupBox = new QGroupBox("Surgery Briefing----------------------");
    doctorCommentGroupBox->setStyleSheet( "border: 1px");
    doctorCommentGroupBox->setFlat(false);
    doctorCommentGroupBox->setFont(QFont("Segoe UI", 8, QFont::DemiBold, true));

    doctorLayout = new QVBoxLayout(doctorCommentGroupBox);
    doctorLayout->addWidget(doctorInformationContainer);
    doctorLayout->addWidget(commentTextEdit);
    doctorLayout->setSpacing(0);

    cancelButton = new CPushButton();
    cancelButton->setText("cancel");
    cancelButton->setFlat(true);
    confirmButton = new CPushButton();
    confirmButton->setText("confirm");
    confirmButton->setFlat(true);
    ccSpacerItem = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    ccContatiner = new QWidget();
    ccLayout = new QHBoxLayout(ccContatiner);
    ccLayout->addItem(ccSpacerItem);
    ccLayout->addWidget(confirmButton);
    ccLayout->addWidget(cancelButton);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(patientInformationContainer);
    mainLayout->addWidget(doctorCommentGroupBox);
    mainLayout->addWidget(ccContatiner);

}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::connections
//!
void PatientInformationWidget::connections(){
    this->connect(cancelButton , SIGNAL(mouseLeftButtonReleased()), this, SLOT(cancel()));
    this->connect(confirmButton , SIGNAL(mouseLeftButtonReleased()), this, SLOT(confirm()));
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::patientInformationConfigured
//! \return
//!
bool PatientInformationWidget::patientInformationConfigured(){
   return this->informationConfigured;
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::getPatientImagePath
//! \return
//!
QString PatientInformationWidget::getPatientImagePath(){
    return this->patientImageLabel->getImagePath();
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::getPatientName
//! \return
//!
QString PatientInformationWidget::getPatientName(){
    return this->nameLineEdit->text().replace(" ", "_");
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::getPatientBirth
//! \return
//!
QString PatientInformationWidget::getPatientBirth(){
    return this->birthdayLineEdit->text().replace(".", "_");
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::getPatientSex
//! \return
//!
QString PatientInformationWidget::getPatientSex(){
    return this->sexualLineEdit->text();
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::getPatientTherapyDate
//! \return
//!
QString PatientInformationWidget::getPatientTherapyDate(){
    return this->therapyTimeLineEdit->text().replace(".", "_");
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::setParentWidget
//! \param p
//!
void PatientInformationWidget::setParentWidget(QObject *p){
    this->parent = p;
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::display
//! \param pos
//!
void PatientInformationWidget::display(QPoint pos){

     if(!displaying){
         this->show();
         this->move(pos.x() + 50, pos.y());
         displaying = true;
     }
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::drawBackground
//!
void PatientInformationWidget::drawBackground(){
 pixmap = new QPixmap(":/images/background_light_green.png");
 QPalette p =  this->palette();

 p.setBrush(QPalette::Background, QBrush(pixmap->scaled(QSize(screen_width*0.35,screen_height*0.5), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

 this->setPalette(p);
 this->setMask(pixmap->mask());
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::confirm
//!
void PatientInformationWidget::confirm(){
    //check the content...
    if(nameLineEdit->text().isEmpty()||birthdayLineEdit->text().isEmpty()){
         QMessageBox *msgBox = new QMessageBox(this);
         msgBox->setWindowTitle("Attention");
         msgBox->setFont(QFont("Segoe UI", 8, QFont::AnyStyle, true));
         msgBox->setStyleSheet("background-color: darkGray; color: AliceBlue");
         msgBox->setText("name and birthday has not filled");
         msgBox->exec();
         return;
    }

    //(qobject_cast<IgssMainWindow *>(this->parent))->createPatient();
    //this->informationConfigured = true;

    //TODO add a listWidgetItem in the listWidget.
    // A drag&drop or upload photo of the patient
    this->close();
    displaying = !displaying;
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::cancel
//!
void PatientInformationWidget::cancel(){
    this->close();
    displaying = !displaying;
    //(qobject_cast<IgssMainWindow *>(this->parent))->reopenMainWindow();
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::mouseMoveEvent
//! \param event
//!
void PatientInformationWidget::mouseMoveEvent(QMouseEvent *event){
    if(mouseLeftButtonPressed == true){
        mouseMovingPosition = event->globalPos();
        this->move(this->pos() + mouseMovingPosition - mousePosition);
        mousePosition = mouseMovingPosition;
    }
    event->ignore();
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::mousePressEvent
//! \param event
//!
void PatientInformationWidget::mousePressEvent(QMouseEvent *event){
 if(event->button() == Qt::LeftButton){
    if (!((event->y() < 5) || (event->x() <5))){
        mousePosition = event->globalPos();
        mouseLeftButtonPressed = true;
    }
 }
 event->ignore();
}

//! --------------------------------------------------------------------------------------------------
//!
//! \brief PatientInformationWidget::mouseReleaseEvent
//! \param event
//!
void PatientInformationWidget::mouseReleaseEvent(QMouseEvent *event){
 if(event->button() == Qt::LeftButton){
     mouseLeftButtonPressed = false;
 }
 event->ignore();
}
