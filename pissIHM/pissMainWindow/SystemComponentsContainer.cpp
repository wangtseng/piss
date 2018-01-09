#include "SystemComponentsContainer.h"

SystemComponentsContainer::SystemComponentsContainer(int width, int height) : QFrame()
{
    setStyleSheet("background-color:transparent; border:0px solid yellow");
    this->width = width;
    this->height = height;

    this->setFixedSize(this->width, this->height);
}

