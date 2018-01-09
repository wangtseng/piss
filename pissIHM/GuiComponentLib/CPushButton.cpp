#include "CPushButton.h"
#include <QDebug>
#include<QMouseEvent>


CPushButton::CPushButton(QWidget *parent) : QPushButton(parent){
    this->setMouseTracking(true);
}

CPushButton::~CPushButton(){}

void CPushButton::pressed(){
    emit mouseLeftButtonReleased();
}

void CPushButton::mouseMoveEvent(QMouseEvent *event){
    emit mouseMove(event->pos());
    event->ignore();
}

void CPushButton::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton)
    {
        emit mouseLeftButtonClicked();
    }
    else{
        emit mouseRightButtonClicked();
    }

    event->ignore();
}

void CPushButton::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton)
    {
        emit mouseLeftButtonReleased();
    }
    else{
        emit mouseRightButtonReleased();
    }

    event->ignore();
}

void  CPushButton::enterEvent(QEvent* event){
    emit mouseHover();
    event->ignore();
}

void CPushButton::leaveEvent(QEvent* event){
    emit mouseLeaved();
    event->ignore();
}

void CPushButton::mouseDoubleClickEvent(QMouseEvent *event){
    emit mouseDoubleClicked();
    event->pos();
}
