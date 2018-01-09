#include <QtWidgets>
#include "igtAnalogMotor.h"


igtAnalogMotor::igtAnalogMotor(QString name, QColor *minuteColor, QColor *roundColor, int width, int height){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->orientationFlag = false;
    rotateTimer = new QTimer();

    this->name = name;
    this->minuteColor = minuteColor;
    this->roundColor = roundColor;
    this->width = width;
    this->height = height;
    this->setFixedSize(QSize(this->width, this->height));

    round = new QLineEdit();
    round->setStyleSheet("border: 0px solid orange");
    round->setFixedSize(this->width*0.2, this->height*0.2);
    round->setFont(QFont("Segoe UI", 18, QFont::AnyStyle, false));
    round->setAlignment(Qt::AlignCenter);

    rpm = new QLineEdit();
    rpm->setStyleSheet("border: 0px solid orange");
    rpm->setFixedSize(this->width*0.2, this->height*0.2);
    rpm->setFont(QFont("Segoe UI", 18, QFont::AnyStyle, false));
    rpm->setAlignment(Qt::AlignCenter);

    QSpacerItem *segmentationObjectComboBoxSpacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);

    parameterName = new QLabel(this->name);
    parameterName->setFixedSize(this->width*0.2, this->height*0.2);
    parameterName->setFont(QFont("Segoe UI", 18, QFont::AnyStyle, false));
    parameterName->setAlignment(Qt::AlignCenter);

    myLayout = new QGridLayout(this);
    myLayout->addWidget(round, 0, 0);
    myLayout->addItem(segmentationObjectComboBoxSpacer, 0, 1);
    myLayout->addWidget(rpm, 0, 2);

    cpt=10;
    this->connect(rotateTimer, SIGNAL(timeout()), this, SLOT(rotate()));
    //rotateTimer->start(100);
}

int igtAnalogMotor::getRotateSpeed(){
    return rpm->text().toInt(0,10);
}

int igtAnalogMotor::getRound(){
    return round->text().toInt(0, 10);
}

void igtAnalogMotor::rotate(){
    int ret;
    if(this->orientationFlag == false){
        cpt+=1;//! to be defined by real angle!!!
     }
    else {
        if(cpt == 0){
            return;
        }
        cpt-=1;
    }
    ret = cpt;
    QString temp = QString::number(ret);
    rpm->setText(temp);

    this->update();
}

void igtAnalogMotor::mousePressEvent(QMouseEvent *event){

}

void igtAnalogMotor::mouseMoveEvent(QMouseEvent *event)
{
//    if (event->buttons() & Qt::LeftButton) {
//        move(event->globalPos() - dragPosition);
//        event->accept();
//    }
}

void igtAnalogMotor::paintEvent(QPaintEvent *event){
    static const QPoint minuteHand[3] = {
        QPoint(3, 0),
        QPoint(-3, 0),
        QPoint(0, -85)
    };


    int side = qMin(width, height);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(*minuteColor);

    painter.save();
    painter.rotate(cpt%360);
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    painter.setPen(*roundColor);

    for (int j = 0; j < 180; ++j) {
        painter.drawLine(93, 0, 96, 0);
        painter.rotate(2);
    }
    event->accept();
}

void igtAnalogMotor::resizeEvent(QResizeEvent * /* event */){
    int side = qMin(width, height);
    QRegion maskedRegion(width / 2 - side / 2, height / 2 - side / 2, side,
                         side, QRegion::Ellipse);
    setMask(maskedRegion);
}

QSize igtAnalogMotor::sizeHint() const{
    return QSize(100, 100);
}

void igtAnalogMotor::rotateSpeedAddition(){
    this->orientationFlag = false;
    this->rotate();

}

void igtAnalogMotor::rotateSpeedSubtraction(){
    this->orientationFlag = true;
    this->rotate();
}

void igtAnalogMotor::stop(){
    this->rotateTimer->stop();
}

void igtAnalogMotor::rotateAngle(int angle){
    cpt = angle;
    QString temp = QString::number(cpt);
    parameterName->setText(temp);

    cpt = cpt*180.0/40;

    this->update();
}
